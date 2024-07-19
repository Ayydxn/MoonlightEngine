#include "MoonlightPCH.h"
#include "VulkanSwapChain.h"
#include "Application/Application.h"
#include "Application/Window.h"

#include <GLFW/glfw3.h>

#include "VulkanDevice.h"

void CVulkanSwapChain::Initialize(const vk::Instance& VulkanInstance, const vk::PhysicalDevice& PhysicalDevice, const vk::Device& LogicalDevice)
{
    /*-------------------------------*/
    /* -- Create a Window Surface -- */
    /*-------------------------------*/
    
    verifyEnginef(glfwCreateWindowSurface(VulkanInstance, static_cast<GLFWwindow*>(CApplication::GetInstance().GetWindow().GetHandle()), nullptr,
        reinterpret_cast<VkSurfaceKHR*>(&m_WindowSurface)) == VK_SUCCESS, "Failed to create Vulkan window surface!")

    m_VulkanInstance = VulkanInstance;

    /*----------------------------------------------------------*/
    /* -- Check if the physical device supports presentation -- */
    /*----------------------------------------------------------*/

    vk::Bool32 bDoesPhysicalDeviceSupportSurfaces = VK_FALSE;
    for (int i = 0; i < static_cast<int32>(PhysicalDevice.getQueueFamilyProperties().size()); i++)
    {
        bDoesPhysicalDeviceSupportSurfaces = PhysicalDevice.getSurfaceSupportKHR(i, m_WindowSurface);
        if (bDoesPhysicalDeviceSupportSurfaces == VK_TRUE)
        {
            m_PresentFamily = i;

            break;
        }
    }
    
    verifyEnginef(bDoesPhysicalDeviceSupportSurfaces != VK_FALSE, "The physical device ({}) doesn't support presenting to a window surface!",
        PhysicalDevice.getProperties().deviceName.data())

    SelectImageFormatAndColorSpace(PhysicalDevice);

    m_PhysicalDevice = PhysicalDevice;
    m_LogicalDevice = LogicalDevice;
    
    bIsInitialized = true;
}

void CVulkanSwapChain::Create(uint32* Width, uint32* Height, bool bEnableVSync)
{
    verifyEnginef(bIsInitialized, "You must initialize a Vulkan swap chain before creating it!")
    
    m_SwapChainState.bIsVSyncEnabled = bEnableVSync;

    const auto [ GraphicsFamily ] = CQueueFamilyIndices::FindQueueFamilies(m_PhysicalDevice);
    const uint32 QueueFamilies[] = { GraphicsFamily, m_PresentFamily };
    const std::vector<vk::PresentModeKHR> PresentModes = m_PhysicalDevice.getSurfacePresentModesKHR(m_WindowSurface);
    const vk::SurfaceCapabilitiesKHR SurfaceCapabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(m_WindowSurface);
    const vk::SwapchainKHR OldSwapChain = m_SwapChain;
    vk::Extent2D SwapChainExtent;

    // If the width (and height) the extent of the surface capabilities is the max value of an uint32, the extent will use the width and height arguments.
    if (SurfaceCapabilities.currentExtent.width == UINT32_MAX)
    {
        SwapChainExtent.width = *Width;
        SwapChainExtent.height = *Height;
    }
    else // The extent of the surface capabilities has proper values, we'll use that.
    {
        SwapChainExtent = SurfaceCapabilities.currentExtent;

        *Width = SurfaceCapabilities.currentExtent.width;
        *Height = SurfaceCapabilities.currentExtent.height;
    }

    m_Extent = SwapChainExtent;
    m_SwapChainState.Width = *Width;
    m_SwapChainState.Height = *Height;

    if (*Width == 0 || *Height == 0)
        return;

    /*
     * VK_PRESENT_MODE_FIFO_KHR is guaranteed to be available as per the Vulkan specification.
     *
     * If we aren't enabling VSync, we use it as a fallback in the event that neither VK_PRESENT_MODE_MAILBOX_KHR or VK_PRESENT_MODE_IMMEDIATE_KHR are available.
     */
    vk::PresentModeKHR SwapChainPresentMode = vk::PresentModeKHR::eFifo;

    if (!bEnableVSync)
    {
        for (const vk::PresentModeKHR PresentMode : PresentModes)
        {
            // If mailbox is supported, we use it.
            if (PresentMode == vk::PresentModeKHR::eMailbox)
            {
                SwapChainPresentMode = vk::PresentModeKHR::eMailbox;
                break;
            }

            // If mailbox isn't supported, we fall back to immediate.
            if (PresentMode == vk::PresentModeKHR::eImmediate)
            {
                SwapChainPresentMode = vk::PresentModeKHR::eImmediate;
                break;
            }
        }
    }

    uint32 MinimumSwapChainImageCount = SurfaceCapabilities.minImageCount + 1;
    if (SurfaceCapabilities.maxImageCount > 0 && MinimumSwapChainImageCount > SurfaceCapabilities.maxImageCount)
        MinimumSwapChainImageCount = SurfaceCapabilities.maxImageCount;

    vk::SurfaceTransformFlagBitsKHR SwapChainPreTransform;
    if ((SurfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) == vk::SurfaceTransformFlagBitsKHR::eIdentity)
    {
        SwapChainPreTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    }
    else
    {
        SwapChainPreTransform = SurfaceCapabilities.currentTransform;
    }

    vk::CompositeAlphaFlagBitsKHR SwapChainCompositeAlpha = {};
    const std::vector<vk::CompositeAlphaFlagBitsKHR> CompositeAlphas =
    {
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
        vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
        vk::CompositeAlphaFlagBitsKHR::eInherit,
    };

    for (const vk::CompositeAlphaFlagBitsKHR CompositeAlpha : CompositeAlphas)
    {
        if ((SurfaceCapabilities.supportedCompositeAlpha & CompositeAlpha) == CompositeAlpha)
        {
            SwapChainCompositeAlpha = CompositeAlpha;
            break;
        }
    }

    vk::ImageUsageFlags SwapChainImageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    if ((SurfaceCapabilities.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferSrc) == vk::ImageUsageFlagBits::eTransferSrc)
        SwapChainImageUsage |= vk::ImageUsageFlagBits::eTransferSrc;

    if ((SurfaceCapabilities.supportedUsageFlags & vk::ImageUsageFlagBits::eTransferDst) == vk::ImageUsageFlagBits::eTransferDst)
        SwapChainImageUsage |= vk::ImageUsageFlagBits::eTransferDst;
    
    vk::SwapchainCreateInfoKHR SwapChainCreateInfo;
    SwapChainCreateInfo.sType = vk::StructureType::eSwapchainCreateInfoKHR;
    SwapChainCreateInfo.surface = m_WindowSurface;
    SwapChainCreateInfo.minImageCount = MinimumSwapChainImageCount;
    SwapChainCreateInfo.imageFormat = m_ImageFormat;
    SwapChainCreateInfo.imageColorSpace = m_ColorSpace;
    SwapChainCreateInfo.imageExtent = SwapChainExtent;
    SwapChainCreateInfo.imageArrayLayers = 1;
    SwapChainCreateInfo.imageUsage = SwapChainImageUsage;

    if (GraphicsFamily != m_PresentFamily)
    {
        SwapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        SwapChainCreateInfo.queueFamilyIndexCount = 2;
        SwapChainCreateInfo.pQueueFamilyIndices = QueueFamilies;
    }
    else
    {
        SwapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
        SwapChainCreateInfo.queueFamilyIndexCount = 0;
        SwapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    SwapChainCreateInfo.preTransform = SwapChainPreTransform;
    SwapChainCreateInfo.compositeAlpha = SwapChainCompositeAlpha;
    SwapChainCreateInfo.presentMode = SwapChainPresentMode;
    SwapChainCreateInfo.clipped = VK_TRUE;
    SwapChainCreateInfo.oldSwapchain = OldSwapChain;
    SwapChainCreateInfo.flags = vk::SwapchainCreateFlagsKHR();

    try
    {
        m_SwapChain = m_LogicalDevice.createSwapchainKHR(SwapChainCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to create Vulkan swap chain! ({})", VulkanSystemError.what())
    }

    if (OldSwapChain != VK_NULL_HANDLE)
        m_LogicalDevice.destroySwapchainKHR(OldSwapChain);

    m_Images = m_LogicalDevice.getSwapchainImagesKHR(m_SwapChain);

    CreateImageViews();
}

void CVulkanSwapChain::Destroy() const
{
    m_LogicalDevice.waitIdle();

    for (const vk::ImageView& ImageView : m_ImageViews)
        m_LogicalDevice.destroyImageView(ImageView);
    
    m_LogicalDevice.destroySwapchainKHR(m_SwapChain);
    m_VulkanInstance.destroySurfaceKHR(m_WindowSurface);
}

void CVulkanSwapChain::Present()
{
}

void CVulkanSwapChain::EnableVSync(bool bEnableVSync)
{
    m_SwapChainState.bIsVSyncEnabled = bEnableVSync;

    m_LogicalDevice.waitIdle();

    Create(&m_SwapChainState.Width, &m_SwapChainState.Height, bEnableVSync);
}

void CVulkanSwapChain::SelectImageFormatAndColorSpace(const vk::PhysicalDevice& PhysicalDevice)
{
    const std::vector<vk::SurfaceFormatKHR> SurfaceFormats = PhysicalDevice.getSurfaceFormatsKHR(m_WindowSurface);
    verifyEnginef(!SurfaceFormats.empty(), "The physical device ({}) has no available surface formats!", PhysicalDevice.getProperties().deviceName.data())

    // By "preferred format", we are referring to the VK_FORMAT_B8G8R8A8_UNORM format.
    bool bFoundPreferredFormat = false;

    for (const vk::SurfaceFormatKHR& SurfaceFormat: SurfaceFormats)
    {
        if (SurfaceFormat.format == vk::Format::eB8G8R8A8Unorm)
        {
            m_ImageFormat = SurfaceFormat.format;
            m_ColorSpace = SurfaceFormat.colorSpace;

            bFoundPreferredFormat = true;

            break;
        }
    }
    // If we failed to find VK_FORMAT_B8G8R8A8_UNORM, we use the image format and color space of the first surface format.
    if (!bFoundPreferredFormat)
    {
        m_ImageFormat = SurfaceFormats[0].format;
        m_ColorSpace = SurfaceFormats[0].colorSpace;
    }
}

void CVulkanSwapChain::CreateImageViews()
{
    m_ImageViews.reserve(m_Images.size());

    for (const vk::Image& Image : m_Images)
    {
        vk::ImageViewCreateInfo ImageViewCreateInfo;
        ImageViewCreateInfo.sType = vk::StructureType::eImageViewCreateInfo;
        ImageViewCreateInfo.image = Image;
        ImageViewCreateInfo.viewType = vk::ImageViewType::e2D;
        ImageViewCreateInfo.format = m_ImageFormat;
        ImageViewCreateInfo.components = vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);

        ImageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        ImageViewCreateInfo.subresourceRange.levelCount = 1;
        ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        ImageViewCreateInfo.subresourceRange.layerCount = 1;

        try
        {
            m_ImageViews.push_back(m_LogicalDevice.createImageView(ImageViewCreateInfo));
        }
        catch (const vk::SystemError& VulkanSystemError)
        {
            verifyEnginef(false, "Failed to create Vulkan image view! ({})", VulkanSystemError.what())
        }
    }
}
