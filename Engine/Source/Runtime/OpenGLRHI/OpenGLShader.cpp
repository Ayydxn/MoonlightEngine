#include "MoonlightPCH.h"
#include "OpenGLShader.h"
#include "Application/Application.h"
#include "Engine/EnginePaths.h"
#include "Renderer/ShaderPreProcessor.h"
#include "Utils/FileUtils.h"

#include <glad/glad.h>
#include <shaderc/shaderc.hpp>

namespace
{
    shaderc_shader_kind ConvertMoonlightShaderStageToShaderc(const EShaderStage ShaderStage)
    {
        switch (ShaderStage)
        {
            case EShaderStage::Unknown: return static_cast<shaderc_shader_kind>(-1);
            case EShaderStage::Vertex: return shaderc_vertex_shader;
            case EShaderStage::Fragment: return shaderc_fragment_shader;
            default: verifyEnginef(false, "Failed to convert Moonlight shader stage to Shaderc")
        }

        return static_cast<shaderc_shader_kind>(-1);
    }

    GLenum ConvertMoonlightShaderStageToOpenGL(const EShaderStage ShaderStage)
    {
        switch (ShaderStage)
        {
            case EShaderStage::Unknown: return 0;
            case EShaderStage::Vertex: return GL_VERTEX_SHADER;
            case EShaderStage::Fragment: return GL_FRAGMENT_SHADER;
            default: verifyEnginef(false, "Failed to convert Moonlight shader stage to OpenGL")
        }

        return 0;
    }

    std::filesystem::path GetShaderStageCacheFileExtension(const EShaderStage ShaderStage)
    {
        switch (ShaderStage)
        {
            // mlsc = Moonlight Shader Cache
            case EShaderStage::Unknown: return "UnknownCache.mlsc";
            case EShaderStage::Vertex: return "VertexCache.mlsc";
            case EShaderStage::Fragment: return "FragmentCache.mlsc";
        }

        verifyEnginef(false, "Failed to get shader cache file extension for shader stage!")
        return "UnknownStage.msc";
    }
}

FOpenGLShader::FOpenGLShader(const std::filesystem::path& Filepath)
    : m_ShaderFilepath(Filepath)
{
    const std::string ShaderSources = FFileUtils::ReadFile(Filepath);
    const std::unordered_map<EShaderStage, std::string> PreProcessedShaderSources = FShaderPreProcessor::PreProcessShader(ShaderSources);

    CompileShaderToSPIRV(PreProcessedShaderSources);
    CreateShaderProgram();
}

FOpenGLShader::~FOpenGLShader()
{
    glDeleteProgram(m_ShaderProgramHandle);
}

void FOpenGLShader::Bind() const
{
    glUseProgram(m_ShaderProgramHandle);
}

void FOpenGLShader::CompileShaderToSPIRV(const std::unordered_map<EShaderStage, std::string>& PreProcessedShaderSources)
{
    const shaderc::Compiler ShaderCompiler;

    shaderc::CompileOptions ShaderCompilerOptions;
    ShaderCompilerOptions.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
    ShaderCompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

    for (auto&& [ShaderStage, ShaderSource] : PreProcessedShaderSources)
    {
        std::string ApplicationName = FApplication::GetInstance().GetSpecification().Name;

        // Remove any spaces that may be present in the application's name.
        ApplicationName.erase(std::ranges::remove_if(ApplicationName, isspace).begin(), ApplicationName.end());
        
        std::filesystem::path ShaderFilepath = m_ShaderFilepath;
        std::filesystem::path ShaderCachePath = FEnginePaths::GetShaderCacheDirectory();
        std::filesystem::path ShaderCacheFilepath = ShaderCachePath / (ApplicationName + "_" + m_ShaderFilepath.stem().string() + "_" +
            GetShaderStageCacheFileExtension(ShaderStage).string());

        // If the shader has an existing cache file, we read and use the SPIRV code from that cache file.
        // Otherwise, we compile the shader and write the resulting SPIRV to a cache file.
        std::ifstream FileReader(ShaderCacheFilepath.string(), std::ios::in | std::ios::binary);
        if (FileReader.is_open())
        {
            FileReader.seekg(0, std::ios::end);

            auto CurrentCharacterPosition = FileReader.tellg();

            FileReader.seekg(0, std::ios::beg);

            auto& ShaderStageSPIRV = m_ShaderSPIRV[ConvertMoonlightShaderStageToOpenGL(ShaderStage)];
            ShaderStageSPIRV.resize(CurrentCharacterPosition / sizeof(uint32));

            FileReader.read(reinterpret_cast<char*>(ShaderStageSPIRV.data()), CurrentCharacterPosition);
        }
        else
        {
            const shaderc::SpvCompilationResult ShaderModule = ShaderCompiler.CompileGlslToSpv(ShaderSource, ConvertMoonlightShaderStageToShaderc(ShaderStage),
            m_ShaderFilepath.string().c_str(), ShaderCompilerOptions);

            if (ShaderModule.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                ENGINE_LOG_ERROR_TAG("Renderer", "Failed to compile shader '{}'!\n", m_ShaderFilepath);
                ENGINE_LOG_ERROR_TAG("Renderer", "  {}", ShaderModule.GetErrorMessage());
            }

            m_ShaderSPIRV[ConvertMoonlightShaderStageToOpenGL(ShaderStage)] = std::vector<uint32>(ShaderModule.cbegin(), ShaderModule.cend());

            std::ofstream FileWriter(ShaderCacheFilepath, std::ios::out | std::ios::binary);
            if (FileWriter.is_open())
            {
                auto& ShaderStageSPIRV = m_ShaderSPIRV[ConvertMoonlightShaderStageToOpenGL(ShaderStage)];

                FileWriter.write(reinterpret_cast<char*>(ShaderStageSPIRV.data()), static_cast<int64>(ShaderStageSPIRV.size()) * sizeof(uint32));
                FileWriter.flush();
                FileWriter.close();
            }
        }
    }
}

void FOpenGLShader::CreateShaderProgram()
{
    m_ShaderProgramHandle = glCreateProgram();

    std::vector<uint32> ShaderIDs;

    for (auto&& [ShaderStage, ShaderSPIRV] : m_ShaderSPIRV)
    {
        GLuint ShaderID = ShaderIDs.emplace_back(glCreateShader(ShaderStage));

        glShaderBinary(1, &ShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, ShaderSPIRV.data(), static_cast<int32>(ShaderSPIRV.size()) * sizeof(uint32));
        glSpecializeShader(ShaderID, "main", 0, nullptr, nullptr);
        glAttachShader(m_ShaderProgramHandle, ShaderID);
    }

    glLinkProgram(m_ShaderProgramHandle);

    int32 ShaderProgramLinkStatus;
    glGetProgramiv(m_ShaderProgramHandle, GL_LINK_STATUS, &ShaderProgramLinkStatus);

    if (ShaderProgramLinkStatus == GL_FALSE)
    {
        int32 InfoLogLength;
        glGetProgramiv(m_ShaderProgramHandle, GL_INFO_LOG_LENGTH, &InfoLogLength);

        if (InfoLogLength > 0)
        {
            std::vector<GLchar> InfoLog(InfoLogLength);
            glGetProgramInfoLog(m_ShaderProgramHandle, InfoLogLength, &InfoLogLength, InfoLog.data());

            ENGINE_LOG_ERROR_TAG("Renderer", "Failed to link OpenGL shader program for shader '{}'!", m_ShaderFilepath);
            ENGINE_LOG_ERROR_TAG("Renderer", "  {}", InfoLog.data());
        }
        else // If InfoLogLength is 0, the shader program has no information log. Therefore, there isn't an error message.
        {
            ENGINE_LOG_ERROR_TAG("Renderer", "Failed to link OpenGL shader program for shader '{}'!", m_ShaderFilepath);
            ENGINE_LOG_ERROR_TAG("Renderer", "  No error message was generated.");
        }

        glDeleteProgram(m_ShaderProgramHandle);

        for (const uint32 ShaderID : ShaderIDs)
            glDeleteShader(ShaderID);

        return;
    }

    for (const uint32 ShaderID : ShaderIDs)
    {
        glDetachShader(m_ShaderProgramHandle, ShaderID);
        glDeleteShader(ShaderID);
    }
}
