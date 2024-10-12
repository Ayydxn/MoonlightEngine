#include "MoonlightPCH.h"
#include "VertexBuffer.h"
#include "OpenGLRHI/OpenGLVertexBuffer.h"
#include "Renderer/Renderer.h"

std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(const void* Data, uint64 Size, EVertexBufferUsage Usage)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLVertexBuffer>(Data, Size, Usage);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create vertex buffer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create vertex buffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create vertex buffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create vertex buffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create vertex buffer! Unknown/unsupported graphics API!")
    return nullptr;
}

std::shared_ptr<IVertexBuffer> IVertexBuffer::Create(uint64 Size, EVertexBufferUsage Usage)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLVertexBuffer>(Size, Usage);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create vertex buffer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create vertex buffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create vertex buffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create vertex buffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create vertex buffer! Unknown/unsupported graphics API!")
    return nullptr;
}
