#include "MoonlightPCH.h"
#include "RendererContext.h"
#include "OpenGLRHI/OpenGLContext.h"
#include "Renderer/Renderer.h"

std::shared_ptr<FRendererContext> FRendererContext::Create(void* WindowHandle)
{
    switch (FRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<FOpenGLContext>(WindowHandle);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create renderer context! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create renderer context! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create renderer context! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create renderer context! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create renderer context! Unknown/unsupported graphics API!")
    return nullptr;
}
