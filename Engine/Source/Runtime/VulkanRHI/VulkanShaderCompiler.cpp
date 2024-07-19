#include "MoonlightPCH.h"
#include "VulkanShaderCompiler.h"
#include "Application/Application.h"
#include "Misc/EnginePaths.h"
#include "Renderer/Shaders/ShaderUtils.h"
#include "Utils/FileUtils.h"

#include <vulkan/vulkan_core.h>

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

void CVulkanShaderCompiler::Initialize()
{
    m_ShaderCompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, VK_API_VERSION_1_2);
    m_ShaderCompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
}

std::vector<uint32> CVulkanShaderCompiler::CompileShader(const std::string& ShaderName, const std::string& ShaderSource, EShaderStage ShaderStage)
{
    ENGINE_LOG_INFO_TAG("Renderer", "Compiling shader '{}'...", ShaderName);
    
    const shaderc::SpvCompilationResult ShaderSPIRV = m_ShaderCompilerHandle.CompileGlslToSpv(ShaderSource.c_str(), ShaderSource.size(),
        ConvertMoonlightShaderStageToShaderc(ShaderStage), ShaderName.c_str(), m_ShaderCompilerOptions);

    if (ShaderSPIRV.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to compile shader '{}'!", ShaderName);
        ENGINE_LOG_ERROR_TAG("Renderer", "  {}", ShaderSPIRV.GetErrorMessage());
    }

    return { ShaderSPIRV.cbegin(), ShaderSPIRV.cend() };
}

// FIXME: (Ayydxn) Shader caching should be optional and not something that is forced to happen.
std::vector<uint32> CVulkanShaderCompiler::CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, const EShaderStage ShaderStage)
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
        FileReader.seekg(0, std::ios::end);

        const auto FileSize = FileReader.tellg();
        
        FileReader.seekg(0, std::ios::beg);

        std::vector<uint32> ShaderSPIRV = {};
        ShaderSPIRV.resize(FileSize / sizeof(uint32));
        
        FileReader.read(reinterpret_cast<char*>(ShaderSPIRV.data()), FileSize);

        return ShaderSPIRV;
    }

    /*--------------------------------------------------------------------------------------------*/
    /* -- If a cache file for the shader doesn't exist, we compile it and create a cache file. -- */
    /*--------------------------------------------------------------------------------------------*/
    
    std::vector<uint32> ShaderSPIRV = CompileShader(ShaderFilepath.stem().string(), CFileUtils::ReadFile(ShaderFilepath), ShaderStage);

    std::ofstream FileWriter(ShaderCacheFilepath, std::ios::out | std::ios::binary);
    if (FileWriter.is_open())
    {
        FileWriter.write(reinterpret_cast<char*>(ShaderSPIRV.data()), static_cast<int64>(ShaderSPIRV.size()) * sizeof(uint32));
        FileWriter.flush();
        FileWriter.close();
    }

    FileWriter.close();

    return ShaderSPIRV;
}
