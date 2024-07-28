#pragma once

#include "VulkanIncludes.h"
#include "RHICore/CommandBuffer.h"

class CVulkanCommandBuffer : public ICommandBuffer
{
public:
    CVulkanCommandBuffer(uint32 Count);
    ~CVulkanCommandBuffer() override;

    void Begin() override;
    void End() override;
    void Submit() override;

    void* GetActiveCommandBuffer() const override { return m_ActiveCommandBuffer; }
private:
    void CreateCommandPool();
    void AllocateCommandBuffers(uint32 CommandBufferCount);
private:
    std::vector<vk::CommandBuffer> m_CommandBuffers;
    vk::CommandBuffer m_ActiveCommandBuffer;
    vk::CommandPool m_CommandPool;
    
    vk::Device m_LogicalDevice;
};
