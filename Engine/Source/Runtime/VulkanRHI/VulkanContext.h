#pragma once

#include "VulkanDevice.h"
#include "VulkanIncludes.h"
#include "VulkanSwapChain.h"
#include "RHICore/RendererContext.h"

class CVulkanContext : public IRendererContext 
{
public:
    CVulkanContext(void* WindowHandle);
    ~CVulkanContext() override;

    void Initialize() override;
    void SwapBuffers() override;
    void SetVSync(bool bEnableVSync) override;

    static std::vector<const char*> GetAvailableValidationLayers();
    static bool AreValidationLayersEnabled();
private:
    void CreateDebugMessenger();
    void PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& DebugMessengerCreateInfo);
    
    std::vector<const char*> GetRequiredInstanceExtensions();
private:
    inline static bool bEnableValidationLayers;
    
    std::shared_ptr<CVulkanPhysicalDevice> m_PhysicalDevice;
    std::shared_ptr<CVulkanLogicalDevice> m_LogicalDevice;
    std::shared_ptr<CVulkanSwapChain> m_VulkanSwapChain;
    
    vk::Instance m_VulkanInstance;
    vk::DebugUtilsMessengerEXT m_DebugMessenger;
    
    void* m_WindowHandle;
};
