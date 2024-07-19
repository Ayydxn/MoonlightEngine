#pragma once

#include "RHICore/ShaderCompiler.h"

#include <shaderc/shaderc.hpp>

class CVulkanShaderCompiler : public IShaderCompiler
{
public:
    CVulkanShaderCompiler() = default;
    ~CVulkanShaderCompiler() override = default;

    void Initialize() override;
    void Destroy() override {}

    std::vector<uint32> CompileShader(const std::string& ShaderName, const std::string& ShaderSource, EShaderStage ShaderStage) override;
    std::vector<uint32> CompileShaderFromFile(const std::filesystem::path& ShaderFilepath, EShaderStage ShaderStage) override;
private:
    shaderc::Compiler m_ShaderCompilerHandle;
    shaderc::CompileOptions m_ShaderCompilerOptions;
};
