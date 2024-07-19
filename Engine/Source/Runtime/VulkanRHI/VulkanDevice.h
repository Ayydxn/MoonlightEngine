#pragma once

#include "VulkanIncludes.h"
#include "Core/CoreTypes.h"

struct CQueueFamilyIndices
{
    uint32 GraphicsFamily = static_cast<uint32>(-1);

    bool IsComplete() const;
};

class CVulkanPhysicalDevice
{
public:
    CVulkanPhysicalDevice(const vk::Instance& VulkanInstance);
    
    CQueueFamilyIndices FindQueueFamilies() const;
    
    const vk::PhysicalDevice& GetHandle() const { return m_PhysicalDevice; }
private:
    bool IsPhysicalDeviceSuitable(const vk::PhysicalDevice& PhysicalDevice);
    bool DoesPhysicalDeviceSupportRequiredExtensions(const vk::PhysicalDevice& PhysicalDevice);

    std::string GetPhysicalDeviceVendorName(int32 VendorID);
    std::string UnpackDriverVersion(int32 VendorID, int32 DriverVersion);
    std::string UnpackVulkanAPIVersion(int32 VulkanAPIVersion);
private:
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
