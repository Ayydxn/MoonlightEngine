#pragma once

#include "VulkanIncludes.h"
#include "RHICore/GraphicsPipeline.h"

class CVulkanGraphicsPipeline : public IGraphicsPipeline
{
public:
    CVulkanGraphicsPipeline(const CGraphicsPipelineSpecification& Specification);
    ~CVulkanGraphicsPipeline() override;

    void Invalidate() override;
private:
    void CreatePipelineLayoutAndCache();
private:
    vk::Device m_LogicalDevice;
    vk::PipelineLayout m_GraphicsPipelineLayout;
    vk::PipelineCache m_GraphcisPipelineCache;
    vk::Pipeline m_GraphicsPipelineHandle;
    
    CGraphicsPipelineSpecification m_PipelineSpecification;
};
