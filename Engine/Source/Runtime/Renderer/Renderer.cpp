#include "MoonlightPCH.h"
#include "Renderer.h"

void CRenderer::PreInitialize()
{
    // TODO: (Ayydan) The graphics API we use should come from a configuration file.
    // But for now, we'll force Vulkan since it's the only one we support.
    m_GraphicsAPI = EGraphicsAPI::Vulkan;
}

void CRenderer::Initialize()
{
}

void CRenderer::Shutdown()
{
}

void CRenderer::BeginFrame()
{
}

void CRenderer::EndFrame()
{
}

std::string CRenderer::GetGraphicsAPIString()
{
    switch (m_GraphicsAPI)
    {
        case EGraphicsAPI::OpenGL: return "OpenGL";
        case EGraphicsAPI::Vulkan: return "Vulkan";
        case EGraphicsAPI::Direct3D11: return "DirectX 11";
        case EGraphicsAPI::Direct3D12: return "DirectX 12";
        case EGraphicsAPI::Metal: return "Metal";
        default: verifyEnginef(false, "Failed to get string for unknown/unsupported graphics API");
    }

    return "Unknown Graphics API";
}
