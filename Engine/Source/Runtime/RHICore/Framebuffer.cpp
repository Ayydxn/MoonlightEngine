#include "MoonlightPCH.h"
#include "Framebuffer.h"
#include "OpenGLRHI/OpenGLFramebuffer.h"
#include "Renderer/Renderer.h"

std::shared_ptr<IFramebuffer> IFramebuffer::Create(const CFramebufferSpecification& Specification)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLFramebuffer>(Specification);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create framebuffer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create framebuffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create framebuffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create framebuffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create framebuffer! Unknown/unsupported graphics API!")
    return nullptr;
}
