#pragma once

#include "VulkanCommandBuffer.h"
#include "VulkanIncludes.h"

class CVulkanSwapChain
{
public:
    CVulkanSwapChain() = default;
    
    void Initialize(const vk::Instance& VulkanInstance, const vk::PhysicalDevice& PhysicalDevice, const vk::Device& LogicalDevice);
    void Create(uint32* Width, uint32* Height, bool bEnableVSync);
    void Destroy() const;

    void Present();
    void EnableVSync(bool bEnableVSync);

    void OnWindowResize(uint32 NewWidth, uint32 NewHeight);

    uint32 GetImageCount() const { return static_cast<uint32>(m_Images.size()); }
    vk::Format GetImageFormat() const { return m_ImageFormat; }
    const vk::Extent2D& GetExtent() const { return m_Extent; }
    const vk::CommandBuffer& GetCurrentCommandBuffer() const { return m_CommandBuffers[m_CurrentFrameIndex]; }
    uint32 GetCurrentFrameIndex() const { return m_CurrentFrameIndex; }
private:
    struct CSwapChainState
    {
        uint32 Width, Height;
        bool bIsVSyncEnabled;
    };

    void SelectImageFormatAndColorSpace(const vk::PhysicalDevice& PhysicalDevice);
    void CreateImageViews();
    void CreateCommandBuffers(uint32 GraphicsFamily);
    void CreateSynchronizationObjects();
private:
    CSwapChainState m_SwapChainState = {};
    
    std::vector<vk::Image> m_Images;
    std::vector<vk::ImageView> m_ImageViews;
    std::vector<vk::CommandBuffer> m_CommandBuffers;
    std::vector<vk::Semaphore> m_PresentCompleteSemaphores;
    std::vector<vk::Semaphore> m_RenderCompleteSemaphores;
    std::vector<vk::Fence> m_WaitFences;
    vk::CommandPool m_CommandPool;
    vk::SurfaceKHR m_WindowSurface;
    vk::Format m_ImageFormat;
    vk::ColorSpaceKHR m_ColorSpace;
    vk::Extent2D m_Extent;
    vk::SwapchainKHR m_SwapChain;

    vk::Instance m_VulkanInstance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_LogicalDevice;
    
    uint32 m_PresentFamily = -1;
    uint32 m_CurrentFrameIndex = 0;
    
    bool bIsInitialized = false;
};
