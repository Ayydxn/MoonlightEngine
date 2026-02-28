#include "MoonlightPCH.h"
#include "OpenGLShader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shaders/ShaderUtils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <ranges>

namespace
{
    uint32 ConvertSlangStageToOpenGL(const SlangStage SlangStage)
    {
        switch (SlangStage)
        {
            case SLANG_STAGE_VERTEX: return GL_VERTEX_SHADER;
            case SLANG_STAGE_FRAGMENT: return GL_FRAGMENT_SHADER;
            default:
            {
                verifyEnginef(false, "Failed to convert Slang shader stage to OpenGL!")
                return -1;
            }
        }
    }
}

COpenGLShader::COpenGLShader(const std::string& Name, const std::string& Source)
{
    const std::shared_ptr<CShaderCompiler> ShaderCompiler = CRenderer::GetShaderCompiler();
    ShaderCompiler->CompileShader(Name, Source, m_CompiledShaderStages);
    
    CreateShaderProgram();
}

COpenGLShader::COpenGLShader(const std::filesystem::path& Filepath)
{
    const std::shared_ptr<CShaderCompiler> ShaderCompiler = CRenderer::GetShaderCompiler();
    ShaderCompiler->CompileShaderFromFile(Filepath, m_CompiledShaderStages);
    
    CreateShaderProgram();
}

COpenGLShader::~COpenGLShader()
{
    glDeleteProgram(m_ShaderProgramHandle);
}

void COpenGLShader::Bind() const
{
    glUseProgram(m_ShaderProgramHandle);
}

void COpenGLShader::SetInt(const std::string& Name, const int32 Value) const
{
    glProgramUniform1i(m_ShaderProgramHandle, glGetUniformLocation(m_ShaderProgramHandle, Name.c_str()), Value);
}

void COpenGLShader::SetIntArray(const std::string& Name, int32* Values, int32 ValueCount) const
{
    glProgramUniform1iv(m_ShaderProgramHandle, glGetUniformLocation(m_ShaderProgramHandle, Name.c_str()), ValueCount, Values);
}

void COpenGLShader::SetFloat(const std::string& Name, float Value) const
{
    glProgramUniform1f(m_ShaderProgramHandle, glGetUniformLocation(m_ShaderProgramHandle, Name.c_str()), Value);
}

void COpenGLShader::SetVector4f(const std::string& Name, const glm::vec4& Value) const
{
    glProgramUniform4f(m_ShaderProgramHandle, glGetUniformLocation(m_ShaderProgramHandle, Name.c_str()), Value.x, Value.y, Value.z, Value.w);
}

void COpenGLShader::SetMatrix4x4f(const std::string& Name, const glm::mat4& Value) const
{
    glProgramUniformMatrix4fv(m_ShaderProgramHandle, glGetUniformLocation(m_ShaderProgramHandle, Name.c_str()), 1, false, glm::value_ptr(Value));
}

void COpenGLShader::CreateShaderProgram()
{
    std::vector<uint32> ShaderIDs;
    
    m_ShaderProgramHandle = glCreateProgram();
    
    for (auto& CompiledShaderStage : m_CompiledShaderStages | std::views::values)
    {
        uint32 ShaderID = ShaderIDs.emplace_back(glCreateShader(ConvertSlangStageToOpenGL(CompiledShaderStage.ShaderStage)));
        
        glShaderBinary(1, &ShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, CompiledShaderStage.Bytecode.data(),
            static_cast<int32>(CompiledShaderStage.Bytecode.size() * sizeof(uint32)));
        glSpecializeShader(ShaderID, "main", 0, nullptr, nullptr);
        
        int32 IsSpecialized = 0;
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &IsSpecialized);

        if (IsSpecialized == GL_FALSE)
        {
            int32 MaxLength = 0;
            glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &MaxLength);

            std::vector<GLchar> ErrorMessage(MaxLength);
            glGetShaderInfoLog(ShaderID, MaxLength, &MaxLength, ErrorMessage.data());

            ENGINE_LOG_ERROR_TAG("Renderer", "Failed to specialize shader '{}':", m_Name);
            ENGINE_LOG_ERROR_TAG("Renderer", "  {}", ErrorMessage.data());
        }
        
        glAttachShader(m_ShaderProgramHandle, ShaderID);
    }
    
    glLinkProgram(m_ShaderProgramHandle);

    int32 IsShaderProgramLinked = 0;
    glGetProgramiv(m_ShaderProgramHandle, GL_LINK_STATUS, &IsShaderProgramLinked);

    if (IsShaderProgramLinked == GL_FALSE)
    {
        int32 MaxLength;
        glGetProgramiv(m_ShaderProgramHandle, GL_INFO_LOG_LENGTH, &MaxLength);

        std::vector<GLchar> ErrorMessage(MaxLength);
        glGetProgramInfoLog(m_ShaderProgramHandle, MaxLength, &MaxLength, ErrorMessage.data());

        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to link OpenGL shader program for shader '{}':", m_Name);
        ENGINE_LOG_ERROR_TAG("Renderer", "  {}", ErrorMessage.data());

        glDeleteProgram(m_ShaderProgramHandle);

        for (uint32 ShaderID : ShaderIDs)
            glDeleteShader(ShaderID);
    }

    for (uint32 ShaderID : ShaderIDs)
    {
        glDetachShader(m_ShaderProgramHandle, ShaderID);
        glDeleteShader(ShaderID);
    }
}
