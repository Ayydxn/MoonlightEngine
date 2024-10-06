#pragma once

#include "RHICore/ShaderCompiler.h"

#include <shaderc/shaderc.hpp>

class COpenGLShaderCompiler : public IShaderCompiler
{
public:
    COpenGLShaderCompiler() = default;
    ~COpenGLShaderCompiler() override = default;

    void Initialize() override;
    void Shutdown() override {}

    void CompileShader(const std::string& ShaderName, const std::string& ShaderSource, EShaderStage ShaderStage, std::vector<uint32>& OutputShaderBytecode) override;
    void CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, EShaderStage ShaderStage, std::vector<uint32>& OutputShaderBytecode) override;
private:
    shaderc::Compiler m_ShaderCompilerHandle;
    shaderc::CompileOptions m_ShaderCompilerOptions;
};
