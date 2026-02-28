#include "MoonlightPCH.h"
#include "UniformBuffer.h"
#include "Renderer/Renderer.h"
#include "OpenGLRHI/OpenGLUniformBuffer.h"

std::shared_ptr<IUniformBuffer> IUniformBuffer::Create(uint32 Size, uint32_t Binding)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLUniformBuffer>(Size, Binding);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create uniform buffer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create uniform buffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create uniform buffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create uniform buffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create uniform buffer! Unknown/unsupported graphics API!")
    return nullptr;
}
