#include "MoonlightPCH.h"
#include "RHIBackend.h"
#include "OpenGLRHI/OpenGLRenderer.h"
#include "Renderer/Renderer.h"

std::shared_ptr<FRHIBackend> FRHIBackend::Create()
{
    switch (FRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<FOpenGLRenderer>();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create RHI backend! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create RHI backend! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create RHI backend! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create RHI backend! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create RHI backend! Unknown/unsupported graphics API!")
    return nullptr;
}
