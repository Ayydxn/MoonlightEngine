#include "MoonlightPCH.h"
#include "ShaderCompiler.h"
#include "ShaderCache.h"
#include "Application/Application.h"
#include "Core/Misc/EnginePaths.h"
#include "Renderer/Renderer.h"
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
    if (!CRenderer::GetConfig().bEnableShaderCaching)
    {
        CompileShader(ShaderFilepath.string(), CFileUtils::ReadFile(ShaderFilepath), CompilationOutput);
        return;
    }

    std::string ApplicationName = CApplication::GetInstance().GetSpecification().Name;
    ApplicationName.erase(std::ranges::remove_if(ApplicationName, isspace).begin(), ApplicationName.end());

    const std::string ShaderSource = CFileUtils::ReadFile(ShaderFilepath);
    const uint64 SourceHash = CShaderCache::ComputeSourceHash(ShaderSource);
    const std::filesystem::path CacheFilepath = CShaderCache::GetCacheFilepath(ApplicationName, ShaderFilepath.stem().string());

    if (CShaderCache::TryLoad(CacheFilepath, SourceHash, CompilationOutput))
        return;

    CompileShader(ShaderFilepath.string(), ShaderSource, CompilationOutput);
    CShaderCache::Write(CacheFilepath, SourceHash, CompilationOutput);
}

void CShaderCompiler::DiagnoseIfNeeded(slang::IBlob* DiagnosticsBlob)
{
    if (DiagnosticsBlob != nullptr)
        ENGINE_LOG_ERROR_TAG("Renderer", static_cast<const char*>(DiagnosticsBlob->getBufferPointer()));
}
