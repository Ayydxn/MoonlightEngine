#include "MoonlightPCH.h"
#include "OpenGLShader.h"
#include "Renderer/Shaders/ShaderUtils.h"
#include "RHICore/ShaderCompiler.h"
#include "Utils/FileUtils.h"

#include <glad/glad.h>

namespace
{
    uint32 ConvertMoonlightShaderStageToOpenGL(const EShaderStage ShaderStage)
    {
        switch (ShaderStage)
        {
            case EShaderStage::Unknown: return -1;
            case EShaderStage::Vertex: return GL_VERTEX_SHADER;
            case EShaderStage::Fragment: return GL_FRAGMENT_SHADER;
        }

        verifyEnginef(false, "Failed to convert Moonlight shader stage to Shaderc!")
        return -1;
    }
}

COpenGLShader::COpenGLShader(const std::string& Name, const std::string& Source, EShaderStage ShaderStage)
    : m_Name(Name)
{
    IShaderCompiler& ShaderCompiler = IShaderCompiler::GetInstance();
    std::vector<uint32> ShaderBytecode;
    
    ShaderCompiler.CompileShader(Name, Source, ShaderStage, ShaderBytecode);

    m_ShaderStageToBytecode[ShaderStage] = ShaderBytecode;

    CreateShaderProgram();
}

COpenGLShader::COpenGLShader(const std::filesystem::path& Filepath)
    : m_Name(Filepath.stem().string())
{
    std::vector<std::filesystem::path> ShadersToCompile = {};
    IShaderCompiler& ShaderCompiler = IShaderCompiler::GetInstance();

    // In the directory that the shader is presumably located in, we find all of its associated shaders (vertex shaders, fragment shaders, etc.)
    // and add them to a list of shaders that are to be compiled.
    //
    // What is considered to be an "associated shader" is simply the file having the same name as the shader. For example:
    // Passing "Resources/Shaders/MyShader" means that "MyShader.vert.glsl" is associated with it.
    // Passing "Resources/Shaders/MyShader" means that "MyOtherShader.vert.glsl" is not associated with it.
    for (const auto& FileEntry : std::filesystem::directory_iterator(CFileUtils::GetFileParentDirectory(Filepath)))
    {
        const auto FileEntryPath = std::filesystem::path(FileEntry);
        const auto FileEntryName = Filepath.stem().string().substr(0, Filepath.stem().string().find_last_of('.'));

        if (FileEntryName == Filepath.stem().string())
            ShadersToCompile.push_back(FileEntryPath);
    }

    for (const auto& ShaderToCompile : ShadersToCompile)
    {
        EShaderStage ShaderStage = CShaderUtils::GetShaderStageFromFileExtension(ShaderToCompile);
        
        std::vector<uint32> ShaderBytecode;
        ShaderCompiler.CompileShaderFromFile(ShaderToCompile, ShaderStage, ShaderBytecode);
        
        m_ShaderStageToBytecode[ShaderStage] = ShaderBytecode;
    }

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

void COpenGLShader::CreateShaderProgram()
{
    std::vector<uint32> ShaderIDs;
    
    m_ShaderProgramHandle = glCreateProgram();

    for (auto&& [ShaderStage, ShaderBytecode] : m_ShaderStageToBytecode)
    {
        uint32 ShaderID = ShaderIDs.emplace_back(glCreateShader(ConvertMoonlightShaderStageToOpenGL(ShaderStage)));

        glShaderBinary(1, &ShaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, ShaderBytecode.data(), static_cast<int32>(ShaderBytecode.size()) * sizeof(uint32));
        glSpecializeShader(ShaderID, "main", 0, nullptr, nullptr);
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
