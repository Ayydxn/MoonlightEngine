#include "MoonlightPCH.h"
#include "VulkanContext.h"
#include "Application/Application.h"

#include <GLFW/glfw3.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType,
    const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void*)
{
    switch (MessageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        {
            if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
            {
                ENGINE_LOG_WARN_TAG("Vulkan Validation - Performance", CallbackData->pMessage);
                break;
            }
            
            if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            {
                ENGINE_LOG_WARN_TAG("Vulkan Validation - Validation", CallbackData->pMessage);
                break;
            }
        }

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        {
            if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
            {
                ENGINE_LOG_ERROR_TAG("Vulkan Validation - Performance", CallbackData->pMessage);
                break;
            }
            
            if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            {
                ENGINE_LOG_ERROR_TAG("Vulkan Validation - Validation", CallbackData->pMessage);
                break;
            }
        }

        default: break;
    }
    
    return VK_FALSE;
}

CVulkanContext::CVulkanContext(void* WindowHandle)
    : m_WindowHandle(WindowHandle)
{
    #if defined(MOONLIGHT_BUILD_DEBUG) || defined(MOONLIGHT_BUILD_RELEASE)
        bEnableValidationLayers = true;
    #else
        bEnableValidationLayers = false;
    #endif
}

CVulkanContext::~CVulkanContext()
{
    m_LogicalDevice->Destroy();
    
    if (bEnableValidationLayers)
        m_VulkanInstance.destroyDebugUtilsMessengerEXT(m_DebugMessenger);
    
    m_VulkanInstance.destroy();
}

void CVulkanContext::Initialize()
{
    VULKAN_HPP_DEFAULT_DISPATCHER.init();

    const auto InstanceExtensions = GetRequiredInstanceExtensions();
    const auto AvailableValidationLayers = GetAvailableValidationLayers();
    
    vk::ApplicationInfo ApplicationInfo;
    ApplicationInfo.sType = vk::StructureType::eApplicationInfo;
    ApplicationInfo.pApplicationName = CApplication::GetInstance().GetSpecification().Name.c_str();
    ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    ApplicationInfo.pEngineName = "Moonlight Engine";
    ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    ApplicationInfo.apiVersion = VK_API_VERSION_1_2;
    
    vk::InstanceCreateInfo InstanceCreateInfo;
    InstanceCreateInfo.sType = vk::StructureType::eInstanceCreateInfo;
    InstanceCreateInfo.pApplicationInfo = &ApplicationInfo;
    InstanceCreateInfo.enabledExtensionCount = static_cast<uint32>(InstanceExtensions.size());
    InstanceCreateInfo.ppEnabledExtensionNames = InstanceExtensions.data();
    InstanceCreateInfo.enabledLayerCount = 0;
    InstanceCreateInfo.ppEnabledLayerNames = nullptr;
    InstanceCreateInfo.pNext = nullptr;
    InstanceCreateInfo.flags = vk::InstanceCreateFlags();

    #ifdef MOONLIGHT_PLATFORM_MAC
        InstanceCreateInfo.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    #endif

    if (bEnableValidationLayers)
    {
        if (!AvailableValidationLayers.empty())
        {
            vk::DebugUtilsMessengerCreateInfoEXT DebugMessengerCreateInfo;
            PopulateDebugMessengerCreateInfo(DebugMessengerCreateInfo);
            
            InstanceCreateInfo.enabledLayerCount = static_cast<uint32>(AvailableValidationLayers.size());
            InstanceCreateInfo.ppEnabledLayerNames = AvailableValidationLayers.data();
            InstanceCreateInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&DebugMessengerCreateInfo);
        }
        else
        {
            ENGINE_LOG_WARN_TAG("Renderer", "No validation layers were found by the Vulkan Loader. Validation will be unavailable. (VkInstance)");
        }
    }
    
    try
    {
        m_VulkanInstance = vk::createInstance(InstanceCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to create Vulkan instance! ({})", VulkanSystemError.what());
    }

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_VulkanInstance);

    CreateDebugMessenger();

    m_PhysicalDevice = std::make_shared<CVulkanPhysicalDevice>(m_VulkanInstance);
    m_LogicalDevice = std::make_shared<CVulkanLogicalDevice>(m_PhysicalDevice);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(m_LogicalDevice->GetHandle());
}

void CVulkanContext::SwapBuffers()
{
    // TODO: (Ayydxn) Implement.
}

void CVulkanContext::SetVSync(bool bEnableVSync)
{
    // TODO: (Ayydxn) Implement.
}

// (Ayydxn) Thanks Godot <3 (https://github.com/godotengine/godot/blob/master/drivers/vulkan/rendering_context_driver_vulkan.cpp#L163)
std::vector<const char*> CVulkanContext::GetAvailableValidationLayers()
{
    const auto AvailableInstanceLayers = vk::enumerateInstanceLayerProperties();
    
    const std::initializer_list<const char*> PreferredValidationLayers = { "VK_LAYER_KHRONOS_validation" };
    const std::initializer_list<const char*> LunarGValidationLayers = { "VK_LAYER_LUNARG_standard_validation" };
    const std::initializer_list<const char*> GoogleValidationLayers = { "VK_LAYER_GOOGLE_threading", "VK_LAYER_LUNARG_parameter_validation", "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_core_validation", "VK_LAYER_GOOGLE_unique_objects" };

    std::vector<const char*> AvailableValidationLayers = {};
    
    for (const std::initializer_list<const char*>& ValidationLayers : { PreferredValidationLayers, LunarGValidationLayers, GoogleValidationLayers })
    {
        bool bFoundValidationLayers = false;

        for (const char* ValidationLayer : ValidationLayers)
        {
            bFoundValidationLayers = false;
            
            for (const vk::LayerProperties& LayerProperties : AvailableInstanceLayers)
            {
                if (strcmp(LayerProperties.layerName, ValidationLayer) == 0)
                {
                    bFoundValidationLayers = true;
                    break;
                }
            }

            if (!bFoundValidationLayers)
                break;
        }

        if (bFoundValidationLayers)
        {
            AvailableValidationLayers.reserve(ValidationLayers.size());
            
            for (const char* ValidationLayer : ValidationLayers)
                AvailableValidationLayers.push_back(ValidationLayer);

            break;
        }
    }
    
    return AvailableValidationLayers;
}

bool CVulkanContext::AreValidationLayersEnabled()
{
    return bEnableValidationLayers;
}

void CVulkanContext::CreateDebugMessenger()
{
    if (!bEnableValidationLayers)
        return;

    vk::DebugUtilsMessengerCreateInfoEXT DebugMessengerCreateInfo;
    PopulateDebugMessengerCreateInfo(DebugMessengerCreateInfo);

    try
    {
        m_DebugMessenger = m_VulkanInstance.createDebugUtilsMessengerEXT(DebugMessengerCreateInfo);
    }
    catch (const vk::SystemError& VulkanSystemError)
    {
        ENGINE_LOG_ERROR_TAG("Renderer", "Failed to create Vulkan debug messenger! ({})", VulkanSystemError.what());
    }
}

void CVulkanContext::PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& DebugMessengerCreateInfo)
{
    DebugMessengerCreateInfo.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;
    DebugMessengerCreateInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    DebugMessengerCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
    DebugMessengerCreateInfo.pfnUserCallback = DebugMessengerCallback;
    DebugMessengerCreateInfo.pUserData = nullptr;
    DebugMessengerCreateInfo.flags = vk::DebugUtilsMessengerCreateFlagsEXT();
}

std::vector<const char*> CVulkanContext::GetRequiredInstanceExtensions()
{
    uint32 GLFWExtensionCount = 0;
    const char** GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);

    auto RequiredInstanceExtensions = std::vector(GLFWExtensions, GLFWExtensions + GLFWExtensionCount);
    
    if (bEnableValidationLayers)
        RequiredInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    #ifdef MOONLIGHT_PLATFORM_MAC
        RequiredInstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif
 
    return RequiredInstanceExtensions;
}
