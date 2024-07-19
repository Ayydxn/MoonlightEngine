#include "MoonlightPCH.h"
#include "RendererContext.h"
#include "Renderer/Renderer.h"
#include "VulkanRHI/VulkanContext.h"

std::shared_ptr<IRendererContext> IRendererContext::Create(void* WindowHandle)
{
    ENGINE_LOG_INFO_TAG("Renderer", "Creating renderer context... (Chosen Graphcis API: {})", CRenderer::GetGraphicsAPIString());
    
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: verifyEnginef(false, "Failed to create renderer context! OpenGL isn't supported!") return nullptr;
        case EGraphicsAPI::Vulkan: return std::make_shared<CVulkanContext>(WindowHandle);
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create renderer context! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create renderer context! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create renderer context! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create renderer context! Unknown/unsupported graphics API!")
    return nullptr;
}
