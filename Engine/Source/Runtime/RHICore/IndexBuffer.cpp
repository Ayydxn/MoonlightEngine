#include "MoonlightPCH.h"
#include "IndexBuffer.h"
#include "OpenGLRHI/OpenGLIndexBuffer.h"
#include "Renderer/Renderer.h"

std::shared_ptr<FIndexBuffer> FIndexBuffer::Create(const void* BufferData, uint64 BufferSize)
{
    switch (FRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<FOpenGLIndexBuffer>(BufferData, BufferSize);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create index buffer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create index buffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create index buffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create index buffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create index buffer! Unknown/unsupported graphics API!")
    return nullptr;
}
