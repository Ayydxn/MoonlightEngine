#include "MoonlightPCH.h"
#include "OpenGLShaderCompiler.h"
#include "Application/Application.h"
#include "Misc/EnginePaths.h"
#include "Renderer/Shaders/ShaderUtils.h"
#include "Utils/FileUtils.h"

namespace
{
    shaderc_shader_kind ConvertMoonlightShaderStageToShaderc(const EShaderStage ShaderStage)
    {
        switch (ShaderStage)
        {
            case EShaderStage::Unknown: return static_cast<shaderc_shader_kind>(-1);
            case EShaderStage::Vertex: return shaderc_vertex_shader;
            case EShaderStage::Fragment: return shaderc_fragment_shader;
        }

        verifyEnginef(false, "Failed to convert Moonlight shader stage to Shaderc!")
        return static_cast<shaderc_shader_kind>(-1);
    }
}

void COpenGLShaderCompiler::Initialize()
{
    m_ShaderCompilerOptions.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
    m_ShaderCompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
}

void COpenGLShaderCompiler::CompileShader(const std::string& ShaderName, const std::string& ShaderSource, EShaderStage ShaderStage,
    std::vector<uint32>& OutputShaderBytecode)
{
    ENGINE_LOG_INFO_TAG("Renderer", "Compiling shader '{}'...", ShaderName);
    
    const shaderc::SpvCompilationResult ShaderSPIRV = m_ShaderCompilerHandle.CompileGlslToSpv(ShaderSource, ConvertMoonlightShaderStageToShaderc(ShaderStage),
        ShaderName.c_str(), m_ShaderCompilerOptions);

    if (ShaderSPIRV.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to compile shader '{}'!", ShaderName);
        ENGINE_LOG_ERROR_TAG("Renderer", "  {}", ShaderSPIRV.GetErrorMessage());
    }

    OutputShaderBytecode = std::vector(ShaderSPIRV.cbegin(), ShaderSPIRV.cend());
}

void COpenGLShaderCompiler::CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, EShaderStage ShaderStage, std::vector<uint32>& OutputShaderBytecode)
{
    std::string ApplicationName = CApplication::GetInstance().GetSpecification().Name;
    std::string ShaderName = ShaderFilepath.filename().string().substr(0, ShaderFilepath.filename().string().find_first_of('.'));

    // Remove any spaces that may be present in the application's name.
    ApplicationName.erase(std::ranges::remove_if(ApplicationName, isspace).begin(), ApplicationName.end());

    const std::filesystem::path ShaderCachePath = CEnginePaths::GetShaderCacheDirectory();
    const std::filesystem::path ShaderCacheFilepath = ShaderCachePath / (ApplicationName + "_" + ShaderName + "_" +
        CShaderUtils::GetShaderStageCacheFileExtension(ShaderStage).string());

    std::ifstream FileReader(ShaderCacheFilepath.string(), std::ios::in | std::ios::binary);
    if (FileReader.is_open())
    {
        ENGINE_LOG_INFO_TAG("Renderer", "Loading shader '{}' from the shader cache...", ShaderFilepath.string());

        FileReader.seekg(0, std::ios::end);

        const size_t FileSize = FileReader.tellg();

        FileReader.seekg(0, std::ios::beg);
        
        OutputShaderBytecode.resize(FileSize / sizeof(uint32));

        FileReader.read(reinterpret_cast<char*>(OutputShaderBytecode.data()), FileSize);
        return;
    }

    /*--------------------------------------------------------------------------------------------*/
    /* -- If a cache file for the shader doesn't exist, we compile it and create a cache file. -- */
    /*--------------------------------------------------------------------------------------------*/
    
    CompileShader(ShaderFilepath.string(), CFileUtils::ReadFile(ShaderFilepath), ShaderStage, OutputShaderBytecode);

    std::ofstream FileWriter(ShaderCacheFilepath, std::ios::out | std::ios::binary);
    if (FileWriter.is_open())
    {
        FileWriter.write(reinterpret_cast<char*>(OutputShaderBytecode.data()), static_cast<int32>(OutputShaderBytecode.size()) * sizeof(uint32));
        FileWriter.flush();
        FileWriter.close();
    }
}
