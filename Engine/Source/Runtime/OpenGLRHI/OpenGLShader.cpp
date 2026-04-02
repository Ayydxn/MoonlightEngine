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
    m_Name = Name;
    
    const std::shared_ptr<CShaderCompiler> ShaderCompiler = CRenderer::GetShaderCompiler();
    ShaderCompiler->CompileShader(Name, Source, m_CompiledShaderStages);
    
    CreateShaderProgram();
    InitializeSamplers();
}

COpenGLShader::COpenGLShader(const std::filesystem::path& Filepath)
{
    m_Name = Filepath.stem().string();
    
    const std::shared_ptr<CShaderCompiler> ShaderCompiler = CRenderer::GetShaderCompiler();
    ShaderCompiler->CompileShaderFromFile(Filepath, m_CompiledShaderStages);
    
    CreateShaderProgram();
    InitializeSamplers();
}

COpenGLShader::~COpenGLShader()
{
    glDeleteProgram(m_ShaderProgramHandle);
}

void COpenGLShader::Bind() const
{
    glUseProgram(m_ShaderProgramHandle);
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

/**
 * Automatically initializes all sampler uniforms found in the shader program to sequential texture unit indices(0, 1, 2, ...).
 * This is necessary because Slang's OpenGL-targeted SPIR-V output does not honor register(t0) as an automatic sequential
 * binding the way HLSL targeting DirectX does — sampler arrays have no guaranteed starting texture unit when compiled to
 * SPIR-V for OpenGL, so without this, every slot in the array would be uninitialized and sample from an undefined unit.
 */
void COpenGLShader::InitializeSamplers() const
{
	int32 UniformCount = 0;
	glGetProgramiv(m_ShaderProgramHandle, GL_ACTIVE_UNIFORMS, &UniformCount);

	for (int32 i = 0; i < UniformCount; i++)
	{
		char Name[256];
		int32 Size = 0;
		GLenum Type = -1;
		glGetActiveUniform(m_ShaderProgramHandle, i, sizeof(Name), nullptr, &Size, &Type, Name);

		// Only handles sampler array types
		// If needed, extend this to support other sampler types and non-array samplers.
		// Probably instead just query a list of supported sampler types and check against that?
		if (Type != GL_SAMPLER_2D && Type != GL_SAMPLER_CUBE)
			continue;

		const int32 Location = GetUniformLocation(Name);
		if (Location == -1)
			continue;

		// Size is the array length (1 for non-arrays)
		std::vector<int32> Slots(Size);
		std::iota(Slots.begin(), Slots.end(), 0);

		glProgramUniform1iv(m_ShaderProgramHandle, Location, Size, Slots.data());
	}
}

int32 COpenGLShader::GetUniformLocation(const std::string& Name) const
{
    if (const auto It = m_UniformLocationCache.find(Name); It != m_UniformLocationCache.end())
        return It->second;

    const int32 Location = glGetUniformLocation(m_ShaderProgramHandle, Name.c_str());
    m_UniformLocationCache[Name] = Location;
    
    return Location;
}
