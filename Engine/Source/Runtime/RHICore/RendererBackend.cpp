﻿#include "MoonlightPCH.h"
#include "RendererBackend.h"
#include "OpenGLRHI/OpenGLRenderer.h"
#include "Renderer/Renderer.h"

std::shared_ptr<IRendererBackend> IRendererBackend::Create()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLRenderer>();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create renderer backend! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create renderer backend! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create renderer backend! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create renderer backend! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create renderer backend! Unknown/unsupported graphics API!")
    return nullptr;
}
