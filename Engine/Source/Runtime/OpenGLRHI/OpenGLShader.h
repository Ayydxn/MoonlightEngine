#pragma once

#include "RHICore/Shader.h"

class FOpenGLShader : public FShader
{
public:
    FOpenGLShader(const std::filesystem::path& Filepath);
    ~FOpenGLShader() override;

    void Bind() const override;
private:
    void CompileShaderToSPIRV(const std::unordered_map<EShaderStage, std::string>& PreProcessedShaderSources);
    void CreateShaderProgram();
private:
    std::unordered_map<uint32, std::vector<uint32>> m_ShaderSPIRV;

    std::string m_ShaderFilepath;
    
    uint32 m_ShaderProgramHandle = 0;
};
