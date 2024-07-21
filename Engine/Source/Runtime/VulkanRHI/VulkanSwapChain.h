#pragma once

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

    vk::Format GetImageFormat() const { return m_ImageFormat; }
    const vk::Extent2D& GetExtent() const { return m_Extent; }
private:
    struct CSwapChainState
    {
        uint32 Width, Height;
        bool bIsVSyncEnabled;
    };

    void SelectImageFormatAndColorSpace(const vk::PhysicalDevice& PhysicalDevice);
    void CreateImageViews();
private:
    CSwapChainState m_SwapChainState = {};

    std::vector<vk::Image> m_Images;
    std::vector<vk::ImageView> m_ImageViews;
    vk::SurfaceKHR m_WindowSurface;
    vk::Format m_ImageFormat;
    vk::ColorSpaceKHR m_ColorSpace;
    vk::Extent2D m_Extent;
    vk::SwapchainKHR m_SwapChain;

    vk::Instance m_VulkanInstance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_LogicalDevice;

    uint32 m_PresentFamily = -1;
    
    bool bIsInitialized = false;
};
