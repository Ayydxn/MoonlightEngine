#pragma once

#include "RHICore/Shader.h"

#include <unordered_map>

class COpenGLShader : public IShader
{
public:
    COpenGLShader(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    COpenGLShader(const std::filesystem::path& Filepath);
    ~COpenGLShader() override;

    void Bind() const;

    const std::string& GetName() const override { return m_Name; }
private:
    void CreateShaderProgram();
private:
    std::unordered_map<EShaderStage, std::vector<uint32>> m_ShaderStageToBytecode;

    std::string m_Name;
    uint32 m_ShaderProgramHandle = 0;
};
