#pragma once

#include "Renderer/Shaders/ShaderCompiler.h"
#include "RHICore/Shader.h"

#include <unordered_map>

class COpenGLShader : public IShader
{
public:
    COpenGLShader(const std::string& Name, const std::string& Source);
    COpenGLShader(const std::filesystem::path& Filepath);
    ~COpenGLShader() override;

    void Bind() const;
private:
    void CreateShaderProgram();
    void InitializeSamplers() const;

    int32 GetUniformLocation(const std::string& Name) const;
private:
    std::unordered_map<SlangStage, CCompiledShaderStage> m_CompiledShaderStages;
    mutable std::unordered_map<std::string, int32> m_UniformLocationCache;
    
    uint32 m_ShaderProgramHandle = 0;
};
