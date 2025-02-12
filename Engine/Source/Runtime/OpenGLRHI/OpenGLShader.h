#pragma once

#include "RHICore/Shader.h"

#include <glm/glm.hpp>

#include <unordered_map>

class COpenGLShader : public IShader
{
public:
    COpenGLShader(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    COpenGLShader(const std::filesystem::path& Filepath);
    ~COpenGLShader() override;

    void Bind() const;

    void SetInt(const std::string& Name, int32 Value) const;
    void SetMatrix4x4f(const std::string& Name, const glm::mat4& Value) const;
private:
    void CreateShaderProgram();
private:
    std::unordered_map<EShaderStage, std::vector<uint32>> m_ShaderStageToBytecode;
    
    uint32 m_ShaderProgramHandle = 0;
};
