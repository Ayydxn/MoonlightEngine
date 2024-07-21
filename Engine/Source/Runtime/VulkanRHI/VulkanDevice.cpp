#include "MoonlightPCH.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"

const std::vector<const char*> RequiredPhysicalDeviceExtensions
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
    VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
};

CQueueFamilyIndices CQueueFamilyIndices::FindQueueFamilies(const vk::PhysicalDevice& PhysicalDevice)
{
    CQueueFamilyIndices QueueFamilyIndices = {};

    const auto QueueFamilyProperties = PhysicalDevice.getQueueFamilyProperties();
    int32 QueueFamilyIndex = 0;

    for (int i = 0; i < static_cast<int32>(QueueFamilyProperties.size()) || !QueueFamilyIndices.IsComplete(); i++)
    {
        const auto& QueueFamilyProperty = QueueFamilyProperties[i];
        
        if ((QueueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
            QueueFamilyIndices.GraphicsFamily = QueueFamilyIndex;

        QueueFamilyIndex++;
    }
    
    return QueueFamilyIndices;
}

CVulkanPhysicalDevice::CVulkanPhysicalDevice(const vk::Instance& VulkanInstance)
{
    const auto AvailablePhysicalDevices = VulkanInstance.enumeratePhysicalDevices();
    verifyEnginef(!AvailablePhysicalDevices.empty(), "Failed to select a Vulkan physical device! No GPUs with Vulkan support were found!")

    for (const auto PhysicalDevice : AvailablePhysicalDevices)
    {
        if (IsPhysicalDeviceSuitable(PhysicalDevice))
        {
            m_PhysicalDevice = PhysicalDevice;
            break;
        }
    }

    verifyEnginef(m_PhysicalDevice, "Failed to find a suitable GPU from those available!")

    vk::PhysicalDeviceProperties PhysicalDeviceProperties = m_PhysicalDevice.getProperties();

    ENGINE_LOG_INFO_TAG("Renderer", "Graphics Card Information:");
    ENGINE_LOG_INFO_TAG("Renderer", "   Device: {}", PhysicalDeviceProperties.deviceName.data());
    ENGINE_LOG_INFO_TAG("Renderer", "   Vendor: {}", GetPhysicalDeviceVendorName(PhysicalDeviceProperties.vendorID));
    ENGINE_LOG_INFO_TAG("Renderer", "   Driver Version: {}", UnpackDriverVersion(PhysicalDeviceProperties.vendorID, PhysicalDeviceProperties.driverVersion));
    ENGINE_LOG_INFO_TAG("Renderer", "   Vulkan API Version: {}", UnpackVulkanAPIVersion(PhysicalDeviceProperties.apiVersion));

    m_DepthFormat = FindDepthFormat();
}

vk::Format CVulkanPhysicalDevice::FindDepthFormat() const
{
    // The list we're iterating through represents potential depth format that we can use.
    for (const vk::Format DepthFormat : { vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint, vk::Format::eD16UnormS8Uint, vk::Format::eD16Unorm })
    {
        const vk::FormatProperties FormatProperties = m_PhysicalDevice.getFormatProperties(DepthFormat);
        if ((FormatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) == vk::FormatFeatureFlagBits::eDepthStencilAttachment)
            return DepthFormat;
    }
    
    return vk::Format::eUndefined;
}

bool CVulkanPhysicalDevice::IsPhysicalDeviceSuitable(const vk::PhysicalDevice& PhysicalDevice)
{
    const vk::PhysicalDeviceProperties PhysicalDeviceProperties = PhysicalDevice.getProperties();
    const bool bIsPhysicalDeviceDiscreteOrIntegrated = PhysicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ||
        PhysicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu;
    const bool bDoesPhysicalDeviceSupportRequiredExtensions = DoesPhysicalDeviceSupportRequiredExtensions(PhysicalDevice);
    const bool bDoesPhysicalDeviceSupportRequiredFeatures = DoesPhysicalDeviceSupportRequiredFeatures(PhysicalDevice);
    
    return bIsPhysicalDeviceDiscreteOrIntegrated && bDoesPhysicalDeviceSupportRequiredExtensions && bDoesPhysicalDeviceSupportRequiredFeatures;
}

bool CVulkanPhysicalDevice::DoesPhysicalDeviceSupportRequiredExtensions(const vk::PhysicalDevice& PhysicalDevice)
{
    const auto AvailablePhysicalDeviceExtensions = PhysicalDevice.enumerateDeviceExtensionProperties();
    std::set<std::string> RequiredExtensions(RequiredPhysicalDeviceExtensions.begin(), RequiredPhysicalDeviceExtensions.end());

    for (const auto& AvailablePhysicalDeviceExtension : AvailablePhysicalDeviceExtensions)
        RequiredExtensions.erase(AvailablePhysicalDeviceExtension.extensionName);

    return RequiredExtensions.empty();
}

bool CVulkanPhysicalDevice::DoesPhysicalDeviceSupportRequiredFeatures(const vk::PhysicalDevice& PhysicalDevice)
{
    vk::PhysicalDeviceVulkan13Features PhysicalDeviceVulkan13Features;
    PhysicalDeviceVulkan13Features.sType = vk::StructureType::ePhysicalDeviceVulkan13Features;

    vk::PhysicalDeviceFeatures2 PhysicalDeviceFeatures2;
    PhysicalDeviceFeatures2.pNext = &PhysicalDeviceVulkan13Features;

    PhysicalDevice.getFeatures2(&PhysicalDeviceFeatures2);

    return PhysicalDeviceVulkan13Features.dynamicRendering && PhysicalDeviceVulkan13Features.synchronization2;
}

std::string CVulkanPhysicalDevice::GetPhysicalDeviceVendorName(int32 VendorID)
{
    switch (VendorID)
    {
        case 0x1002: return "AMD Incorporated";
        case 0x1010: return "Imagination Technologies Limited";
        case 0x10DE: return "NVIDIA Corporation";
        case 0x13B5: return "ARM";
        case 0x5143: return "Qualcomm";
        case 0x8080: return "Intel Corporation";
        default: return "Unknown GPU Vendor";
    }
}

std::string CVulkanPhysicalDevice::UnpackDriverVersion(int32 VendorID, int32 DriverVersion)
{
    if (VendorID == 0x10DE)
    {
        int32 DriverMajorVersion = DriverVersion >> 22 & 0x3ff;
        int32 DriverMinorVersion = DriverVersion >> 14 & 0x0ff;
        int32 DriverPatchVersion = DriverVersion >> 6 & 0x0ff;
        int32 DriverVariantVersion = DriverVersion & 0x003f;

        return std::format("{}.{}.{}.{}", DriverMajorVersion, DriverMinorVersion, DriverPatchVersion, DriverVariantVersion);
    }

    #ifdef MOONLIGHT_PLATFORM_WIN64
        // Intel
        if (VendorID == 0x8086)
        {
            int DriverMajorVersion = DriverVersion >> 14;
            int DriverMinorVersion = DriverVersion & 0x3fff;
        
            return std::format("%d.%d", DriverMajorVersion, DriverMinorVersion);
        }
    #endif

    // Use Vulkan version conventions if a vendor mapping isn't available.
    int DriverMajorVersion = DriverVersion >> 22;
    int DriverMinorVersion = DriverVersion >> 12 & 0x3ff;
    int DriverPatchVersion = DriverVersion & 0xfff;

    return std::format("{}.{}.{}", DriverMajorVersion, DriverMinorVersion, DriverPatchVersion);
}

std::string CVulkanPhysicalDevice::UnpackVulkanAPIVersion(int32 VulkanAPIVersion)
{
    return std::format("{}.{}.{}", VK_API_VERSION_MAJOR(VulkanAPIVersion), VK_API_VERSION_MINOR(VulkanAPIVersion), VK_API_VERSION_PATCH(VulkanAPIVersion));
}

CVulkanLogicalDevice::CVulkanLogicalDevice(const std::shared_ptr<CVulkanPhysicalDevice>& PhysicalDevice)
{
    const CQueueFamilyIndices QueueFamilyIndices = CQueueFamilyIndices::FindQueueFamilies(PhysicalDevice->GetHandle());
    const auto AvailableValidationLayers = CVulkanContext::GetAvailableValidationLayers();
    const std::set<uint32> QueueFamilies = { QueueFamilyIndices.GraphicsFamily };
    constexpr float QueuePriority = 1.0f;

    std::vector<vk::DeviceQueueCreateInfo> DeviceQueueCreateInfos = {};
    
    for (const uint32 QueueFamily : QueueFamilies)
    {
        vk::DeviceQueueCreateInfo DeviceQueueCreateInfo;
        DeviceQueueCreateInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
        DeviceQueueCreateInfo.queueFamilyIndex = QueueFamily;
        DeviceQueueCreateInfo.queueCount = 1;
        DeviceQueueCreateInfo.pQueuePriorities = &QueuePriority;

        DeviceQueueCreateInfos.push_back(DeviceQueueCreateInfo);
    }
    
    vk::PhysicalDeviceVulkan13Features PhysicalDeviceVulkan13Features;
    PhysicalDeviceVulkan13Features.sType = vk::StructureType::ePhysicalDeviceVulkan13Features;

    vk::PhysicalDeviceFeatures2 PhysicalDeviceFeatures;
    PhysicalDeviceFeatures.pNext = &PhysicalDeviceVulkan13Features;

    PhysicalDevice->GetHandle().getFeatures2(&PhysicalDeviceFeatures);

    vk::DeviceCreateInfo LogicalDeviceCreateInfo;
    LogicalDeviceCreateInfo.sType = vk::StructureType::eDeviceCreateInfo;
    LogicalDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(DeviceQueueCreateInfos.size());
    LogicalDeviceCreateInfo.pQueueCreateInfos = DeviceQueueCreateInfos.data();
    LogicalDeviceCreateInfo.enabledExtensionCount = static_cast<uint32>(RequiredPhysicalDeviceExtensions.size());
    LogicalDeviceCreateInfo.ppEnabledExtensionNames = RequiredPhysicalDeviceExtensions.data();
    LogicalDeviceCreateInfo.enabledLayerCount = 0;
    LogicalDeviceCreateInfo.ppEnabledLayerNames = nullptr;
    LogicalDeviceCreateInfo.pNext = &PhysicalDeviceFeatures;
    LogicalDeviceCreateInfo.flags = vk::DeviceCreateFlags();

    if (CVulkanContext::AreValidationLayersEnabled())
    {
        if (!AvailableValidationLayers.empty())
        {
            LogicalDeviceCreateInfo.enabledLayerCount = static_cast<uint32>(AvailableValidationLayers.size());
            LogicalDeviceCreateInfo.ppEnabledLayerNames = AvailableValidationLayers.data();
        }
        else
        {
            ENGINE_LOG_WARN_TAG("Renderer", "No validation layers were found by the Vulkan Loader. Validation will be unavailable. (VkDevice)");
        }
    }

    try
    {
        m_LogicalDevice = PhysicalDevice->GetHandle().createDevice(LogicalDeviceCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        verifyEnginef(false, "Failed to create Vulkan logical device! ({})", VulkanSystemError.what())
    }

    m_GraphicsQueue = m_LogicalDevice.getQueue(QueueFamilyIndices.GraphicsFamily, 0);
}

void CVulkanLogicalDevice::Destroy() const
{
    m_LogicalDevice.waitIdle();
    m_LogicalDevice.destroy();
}
