#pragma once

#include "VulkanIncludes.h"
#include "RHICore/Shader.h"

#include <unordered_map>

class CVulkanShader : public IShader
{
public:
    CVulkanShader(const std::string& Name, const std::string& Source, EShaderStage ShaderStage);
    CVulkanShader(const std::filesystem::path& Filepath);
    ~CVulkanShader() override;
private:
    void CreateShaderModules();
private:
    std::unordered_map<EShaderStage, std::vector<uint32>> m_ShaderStageToSPIRV;
    std::vector<vk::PipelineShaderStageCreateInfo> m_PipelineShaderStageCreateInfos;
    std::vector<vk::ShaderModule> m_ShaderModules;

    vk::Device m_LogicalDevice;
};
