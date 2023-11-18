#include "MoonlightPCH.h"
#include "OpenGLShader.h"
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
}

FOpenGLShader::FOpenGLShader(const std::filesystem::path& Filepath)
    : m_ShaderFilepath(Filepath.string())
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
        const shaderc::SpvCompilationResult ShaderModule = ShaderCompiler.CompileGlslToSpv(
            ShaderSource, ConvertMoonlightShaderStageToShaderc(ShaderStage),
            m_ShaderFilepath.c_str(), ShaderCompilerOptions);

        if (ShaderModule.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            ENGINE_LOG_ERROR_TAG("Renderer", "Failed to compile shader '{}'!\n\n", m_ShaderFilepath);
            ENGINE_LOG_ERROR_TAG("Renderer", "  {}", ShaderModule.GetErrorMessage());
        }

        m_ShaderSPIRV[ConvertMoonlightShaderStageToOpenGL(ShaderStage)] = std::vector<uint32>(ShaderModule.cbegin(), ShaderModule.cend());
    }
}

void FOpenGLShader::CreateShaderProgram()
{
    m_ShaderProgramHandle = glCreateProgram();

    std::vector<GLenum> ShaderIDs;

    for (auto&& [ShaderStage, ShaderSPIRV] : m_ShaderSPIRV)
    {
        GLuint ShaderID = ShaderIDs.emplace_back(glCreateShader(ShaderStage));

        glShaderBinary(1, &ShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, ShaderSPIRV.data(), static_cast<int32>(ShaderSPIRV.size()) * sizeof(uint32));
        glSpecializeShader(ShaderID, "main", 0, nullptr, nullptr);
        glAttachShader(m_ShaderProgramHandle, ShaderID);
    }

    glLinkProgram(m_ShaderProgramHandle);

    for (const uint32 ShaderID : ShaderIDs)
    {
        glDetachShader(m_ShaderProgramHandle, ShaderID);
        glDeleteShader(ShaderID);
    }
}
