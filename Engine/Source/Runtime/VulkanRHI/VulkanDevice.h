#pragma once

#include "VulkanIncludes.h"
#include "Core/CoreTypes.h"

struct CQueueFamilyIndices
{
    static CQueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& PhysicalDevice);

    bool IsComplete() const
    {
        return GraphicsFamily != -1;
    }

    uint32 GraphicsFamily = static_cast<uint32>(-1);
};

class CVulkanPhysicalDevice
{
public:
    CVulkanPhysicalDevice(const vk::Instance& VulkanInstance);

    vk::Format GetDepthFormat() const { return m_DepthFormat; }
    const vk::PhysicalDevice& GetHandle() const { return m_PhysicalDevice; }
private:
    vk::Format FindDepthFormat() const;
    
    bool IsPhysicalDeviceSuitable(const vk::PhysicalDevice& PhysicalDevice);
    bool DoesPhysicalDeviceSupportRequiredExtensions(const vk::PhysicalDevice& PhysicalDevice);
    bool DoesPhysicalDeviceSupportRequiredFeatures(const vk::PhysicalDevice& PhysicalDevice);

    std::string GetPhysicalDeviceVendorName(int32 VendorID);
    std::string UnpackDriverVersion(int32 VendorID, int32 DriverVersion);
    std::string UnpackVulkanAPIVersion(int32 VulkanAPIVersion);
private:
    vk::Format m_DepthFormat;
    vk::PhysicalDevice m_PhysicalDevice;
};

class CVulkanLogicalDevice
{
public:
    CVulkanLogicalDevice(const std::shared_ptr<CVulkanPhysicalDevice>& PhysicalDevice);

    void Destroy() const;
    
    const vk::Device& GetHandle() const { return m_LogicalDevice; }
private:
    vk::Device m_LogicalDevice;
    vk::Queue m_GraphicsQueue;
};
