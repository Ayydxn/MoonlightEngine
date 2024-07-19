#pragma once

#include "RHICore/RendererContext.h"

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

class CVulkanContext : public IRendererContext 
{
public:
    CVulkanContext(void* WindowHandle);
    ~CVulkanContext() override;

    void Initialize() override;
    void SwapBuffers() override;
    void SetVSync(bool bEnableVSync) override;
private:
    void CreateDebugMessenger();
    void PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& DebugMessengerCreateInfo);
    
    std::vector<const char*> GetRequiredInstanceExtensions();
    std::vector<const char*> GetAvailableValidationLayers();
private:
    vk::Instance m_VulkanInstance;
    vk::DebugUtilsMessengerEXT m_DebugMessenger;
    
    void* m_WindowHandle;
};
