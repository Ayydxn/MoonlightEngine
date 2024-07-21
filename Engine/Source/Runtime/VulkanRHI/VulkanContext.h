#pragma once

#include "VulkanDevice.h"
#include "VulkanIncludes.h"
#include "VulkanSwapChain.h"
#include "Renderer/Renderer.h"
#include "RHICore/RendererContext.h"

class CVulkanContext : public IRendererContext 
{
public:
    CVulkanContext(void* WindowHandle);
    ~CVulkanContext() override;

    void Initialize() override;
    void SwapBuffers() override;
    void SetVSync(bool bEnableVSync) override;

    static std::shared_ptr<CVulkanContext> GetInstance() { return std::dynamic_pointer_cast<CVulkanContext>(CRenderer::GetContext()); }
    
    static std::vector<const char*> GetAvailableValidationLayers();
    static bool AreValidationLayersEnabled();

    const std::shared_ptr<CVulkanPhysicalDevice>& GetPhysicalDevice() const { return m_PhysicalDevice; }
    const std::shared_ptr<CVulkanLogicalDevice>& GetLogicalDevice() const { return m_LogicalDevice; }
    const std::shared_ptr<CVulkanSwapChain>& GetSwapChain() const { return m_VulkanSwapChain; }
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
