#include "MoonlightPCH.h"
#include "ShaderCompiler.h"
#include "Application/Application.h"
#include "Core/Misc/EnginePaths.h"
#include "Renderer/Renderer.h"
#include "ShaderUtils.h"
#include "Utils/FileUtils.h"

void CShaderCompiler::Initialize()
{
    // Create Slang global session
    SlangGlobalSessionDesc GlobalSessionDescription = {};
    GlobalSessionDescription.enableGLSL = true;
    
    SLANG_RETURN_VOID_ON_FAIL(slang::createGlobalSession(&GlobalSessionDescription, m_GlobalSession.writeRef()))

    // Create one compilation session that'll be used for all shader compilations
    slang::TargetDesc TargetDescription = {};
    
    if (CRenderer::GetGraphicsAPI() == EGraphicsAPI::OpenGL || CRenderer::GetGraphicsAPI() == EGraphicsAPI::Vulkan)
    {
        TargetDescription.format = SLANG_SPIRV;
        TargetDescription.profile = m_GlobalSession->findProfile("spirv_1_5");
        TargetDescription.flags = 0;
    }
    else if (CRenderer::GetGraphicsAPI() == EGraphicsAPI::Direct3D11 || CRenderer::GetGraphicsAPI() == EGraphicsAPI::Direct3D12)
    {
        TargetDescription.format = SLANG_DXIL;
        TargetDescription.profile = m_GlobalSession->findProfile("sm_6_5");
        TargetDescription.flags = 0;
    }
    else if (CRenderer::GetGraphicsAPI() == EGraphicsAPI::Metal)
    {
        TargetDescription.format = SLANG_METAL;
        TargetDescription.profile = m_GlobalSession->findProfile("metal");
        TargetDescription.flags = 0;
    }
    
    slang::CompilerOptionEntry CompilerOptionEntry;
    CompilerOptionEntry.name = slang::CompilerOptionName::NoMangle;
    CompilerOptionEntry.value.intValue0 = 1;
    CompilerOptionEntry.value.intValue1 = 1;
    
    slang::SessionDesc SessionDescription = {};
    SessionDescription.targets = &TargetDescription;
    SessionDescription.targetCount = 1;
    SessionDescription.compilerOptionEntries = &CompilerOptionEntry;
    SessionDescription.compilerOptionEntryCount = 1;
    
    SLANG_RETURN_VOID_ON_FAIL(m_GlobalSession->createSession(SessionDescription, m_Session.writeRef()))
}

void CShaderCompiler::Shutdown()
{
    
}

void CShaderCompiler::CompileShader(const std::string& ShaderName, const std::string& ShaderSource, std::unordered_map<SlangStage, CCompiledShaderStage>& CompilationOutput)
{
    ENGINE_LOG_INFO_TAG("Renderer", "Compiling shader '{}'...", ShaderName);
    
    // Load the shader modules
    slang::IModule* SlangModule;
    {
        Slang::ComPtr<slang::IBlob> DiagnosticsBlob;
    
        SlangModule = m_Session->loadModuleFromSourceString(ShaderName.c_str(), (ShaderName + ".slang").c_str(), ShaderSource.c_str(),
            DiagnosticsBlob.writeRef());
    
        DiagnoseIfNeeded(DiagnosticsBlob);
    
        if (!SlangModule)
            return;
    }
    
    // Query the entry points and compose both them and the modules
    uint32 EntryPointCount = SlangModule->getDefinedEntryPointCount();
    
    std::vector<slang::IComponentType*> ComponentTypes;
    ComponentTypes.push_back(SlangModule);
    
    std::vector<Slang::ComPtr<slang::IEntryPoint>> EntryPoints;
    
    for (uint32 i = 0; i < EntryPointCount; i++)
    {
        Slang::ComPtr<slang::IEntryPoint> EntryPoint;
        
        SlangModule->getDefinedEntryPoint(static_cast<int32>(i), EntryPoint.writeRef());
        
        EntryPoints.push_back(EntryPoint);
        ComponentTypes.push_back(EntryPoint);
    }
    
    Slang::ComPtr<slang::IComponentType> ComposedProgram;
    {
        Slang::ComPtr<slang::IBlob> DiagnosticsBlob;
        
        const SlangResult Result = m_Session->createCompositeComponentType(ComponentTypes.data(), static_cast<int64>(ComponentTypes.size()),
            ComposedProgram.writeRef(), DiagnosticsBlob.writeRef());
        
        DiagnoseIfNeeded(DiagnosticsBlob);
        
        SLANG_RETURN_VOID_ON_FAIL(Result)
    }
    
    // Retrieve and store the SPIR-V bytecode
    for (uint32 i = 0; i < EntryPointCount; i++)
    {
        Slang::ComPtr<slang::IBlob> DiagnosticsBlob;
        
        slang::IEntryPoint* EntryPoint = EntryPoints[i];
        const char* EntryPointName = EntryPoint->getFunctionReflection()->getName();
        
        Slang::ComPtr<slang::IBlob> BytecodeBlob;
        const SlangResult Result = ComposedProgram->getEntryPointCode(i, 0, BytecodeBlob.writeRef(), DiagnosticsBlob.writeRef());
        
        DiagnoseIfNeeded(DiagnosticsBlob);
        
        SLANG_RETURN_VOID_ON_FAIL(Result)
        
        if (BytecodeBlob)
        {
            const auto Start = static_cast<const uint32*>(BytecodeBlob->getBufferPointer());
            const size_t WordCount = BytecodeBlob->getBufferSize() / sizeof(uint32);
            
            CCompiledShaderStage CompiledShaderStage;
            CompiledShaderStage.ShaderStage =  EntryPoint->getLayout()->getEntryPointByIndex(0)->getStage();
            CompiledShaderStage.EntryPointName = EntryPointName;
            CompiledShaderStage.Bytecode = std::vector(Start, Start + WordCount);
            
            CompilationOutput[CompiledShaderStage.ShaderStage] = std::move(CompiledShaderStage);
        }
    }
}

void CShaderCompiler::CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, std::unordered_map<SlangStage, CCompiledShaderStage>& CompilationOutput)
{
    std::string ApplicationName = CApplication::GetInstance().GetSpecification().Name;
    ApplicationName.erase(std::ranges::remove_if(ApplicationName, isspace).begin(), ApplicationName.end());

    const std::string ShaderName = ShaderFilepath.stem().string();

    if (CRenderer::GetConfig().bEnableShaderCaching)
    {
        const std::filesystem::path ShaderCachePath = CEnginePaths::GetShaderCacheDirectory();

        // Check if ALL cache files for this shader exist before skipping compilation entirely.
        // We don't know how many stages exist without compiling first.
        // So, we compile and then check per-stage whether to load from cache or write a new cache file.
        CompileShader(ShaderFilepath.string(), CFileUtils::ReadFile(ShaderFilepath), CompilationOutput);

        for (auto& [Stage, CompiledStage] : CompilationOutput)
        {
            const std::filesystem::path ShaderCacheFilepath = ShaderCachePath / 
                (ApplicationName + "_" + ShaderName + "_" + CShaderUtils::GetShaderStageCacheFileExtension(Stage).string());

            std::ifstream FileReader(ShaderCacheFilepath, std::ios::in | std::ios::binary);

            if (FileReader.is_open())
            {
                ENGINE_LOG_INFO_TAG("Renderer", "Loading shader '{}' (stage {}) from the shader cache...", ShaderFilepath.string(), static_cast<int32>(Stage));

                FileReader.seekg(0, std::ios::end);
                const size_t FileSize = FileReader.tellg();
                FileReader.seekg(0, std::ios::beg);

                CompiledStage.Bytecode.resize(FileSize / sizeof(uint32));
                FileReader.read(reinterpret_cast<char*>(CompiledStage.Bytecode.data()), static_cast<int64>(FileSize));
            }
            else
            {
                std::ofstream FileWriter(ShaderCacheFilepath, std::ios::out | std::ios::binary);

                if (FileWriter.is_open())
                {
                    ENGINE_LOG_INFO_TAG("Renderer", "Writing shader '{}' (stage {}) to the shader cache...", ShaderFilepath.string(), static_cast<int32>(Stage));

                    FileWriter.write(reinterpret_cast<const char*>(CompiledStage.Bytecode.data()), static_cast<int32>(CompiledStage.Bytecode.size()) * sizeof(uint32));
                    FileWriter.flush();
                    FileWriter.close();
                }
            }
        }
    }
    else
    {
        CompileShader(ShaderFilepath.string(), CFileUtils::ReadFile(ShaderFilepath), CompilationOutput);
    }
}

void CShaderCompiler::DiagnoseIfNeeded(slang::IBlob* DiagnosticsBlob)
{
    if (DiagnosticsBlob != nullptr)
        ENGINE_LOG_ERROR_TAG("Renderer", static_cast<const char*>(DiagnosticsBlob->getBufferPointer()));
}
