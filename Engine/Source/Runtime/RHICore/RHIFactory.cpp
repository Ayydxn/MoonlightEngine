#include "MoonlightPCH.h"
#include "RHIFactory.h"
#include "OpenGLRHI/OpenGLRHIFactory.h"
#include "Renderer/Renderer.h"

std::unique_ptr<IRHIFactory> IRHIFactory::Create()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_unique<COpenGLRHIFactory>();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create a RHI factor! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create a RHI factor! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create a RHI factor! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create a RHI factor! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create a RHI factor! Unknown/unsupported graphics API!")
    return nullptr;
}
