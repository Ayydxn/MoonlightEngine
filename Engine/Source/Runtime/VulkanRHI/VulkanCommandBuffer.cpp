#include "MoonlightPCH.h"
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"

CVulkanCommandBuffer::CVulkanCommandBuffer(uint32 Count)
    : m_LogicalDevice(CVulkanContext::GetInstance()->GetLogicalDevice()->GetHandle())
{
    if (Count == 0)
        Count = CVulkanContext::GetInstance()->GetSwapChain()->GetImageCount();

    m_CommandBuffers.reserve(Count);

    CreateCommandPool();
    AllocateCommandBuffers(Count);
}

CVulkanCommandBuffer::~CVulkanCommandBuffer()
{
    m_LogicalDevice.waitIdle();
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

void CVulkanCommandBuffer::Begin()
{
    const uint32 CurrentImageIndex = CVulkanContext::GetInstance()->GetSwapChain()->GetCurrentFrameIndex();
    const vk::CommandBuffer CommandBuffer = m_CommandBuffers[CurrentImageIndex];

    vk::CommandBufferBeginInfo CommandBufferBeginInfo;
    CommandBufferBeginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
    CommandBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    try
    {
        CommandBuffer.begin(CommandBufferBeginInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to begin Vulkan command buffer #{} ({})", CurrentImageIndex, VulkanSystemError.what())
    }

    m_ActiveCommandBuffer = CommandBuffer;
}

void CVulkanCommandBuffer::End()
{
    const uint32 CurrentImageIndex = CVulkanContext::GetInstance()->GetSwapChain()->GetCurrentFrameIndex();
    const vk::CommandBuffer CommandBuffer = m_CommandBuffers[CurrentImageIndex];

    try
    {
        CommandBuffer.end();
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to end Vulkan command buffer #{} ({})", CurrentImageIndex, VulkanSystemError.what())
    }

    m_ActiveCommandBuffer = nullptr;
}

void CVulkanCommandBuffer::Submit()
{
    const vk::Queue GraphicsQueue = CVulkanContext::GetInstance()->GetLogicalDevice()->GetGraphicsQueue();
    const uint32 CurrentImageIndex = CVulkanContext::GetInstance()->GetSwapChain()->GetCurrentFrameIndex();
    const vk::CommandBuffer CommandBuffer = m_CommandBuffers[CurrentImageIndex];

    vk::SubmitInfo CommandBufferSubmitInfo;
    CommandBufferSubmitInfo.sType = vk::StructureType::eSubmitInfo;
    CommandBufferSubmitInfo.pCommandBuffers = &CommandBuffer;

    try
    {
        GraphicsQueue.submit(CommandBufferSubmitInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to submit Vulkan command buffer! ({})", VulkanSystemError.what())
    }

    GraphicsQueue.waitIdle();
}

void CVulkanCommandBuffer::CreateCommandPool()
{
    const vk::PhysicalDevice PhysicalDevice = CVulkanContext::GetInstance()->GetPhysicalDevice()->GetHandle();
    const CQueueFamilyIndices QueueFamilyIndices = CQueueFamilyIndices::FindQueueFamilies(PhysicalDevice);

    vk::CommandPoolCreateInfo CommandPoolCreateInfo;
    CommandPoolCreateInfo.sType = vk::StructureType::eCommandPoolCreateInfo;
    CommandPoolCreateInfo.queueFamilyIndex = QueueFamilyIndices.GraphicsFamily;
    CommandPoolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    try
    {
        m_CommandPool = m_LogicalDevice.createCommandPool(CommandPoolCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to create Vulkan command pool! ({})", VulkanSystemError.what())
    }
}

void CVulkanCommandBuffer::AllocateCommandBuffers(uint32 CommandBufferCount)
{
    vk::CommandBufferAllocateInfo CommandBufferAllocateInfo;
    CommandBufferAllocateInfo.sType = vk::StructureType::eCommandBufferAllocateInfo;
    CommandBufferAllocateInfo.commandPool = m_CommandPool;
    CommandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
    CommandBufferAllocateInfo.commandBufferCount = CommandBufferCount;

    try
    {
        m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(CommandBufferAllocateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to allocate Vulkan command buffers! ({})", VulkanSystemError.what())
    }
}
