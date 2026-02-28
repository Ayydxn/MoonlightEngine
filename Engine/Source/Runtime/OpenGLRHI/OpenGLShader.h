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

    void SetInt(const std::string& Name, int32 Value) const override;
    void SetIntArray(const std::string& Name, int32* Values, int32 ValueCount) const override;
    void SetFloat(const std::string& Name, float Value) const override;
    void SetVector4f(const std::string& Name, const glm::vec4& Value) const override;
    void SetMatrix4x4f(const std::string& Name, const glm::mat4& Value) const override;
private:
    void CreateShaderProgram();
private:
    std::unordered_map<SlangStage, CCompiledShaderStage> m_CompiledShaderStages;
    
    uint32 m_ShaderProgramHandle = 0;
};
