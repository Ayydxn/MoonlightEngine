#include "MoonlightPCH.h"
#include "IndexBuffer.h"
#include "OpenGLRHI/OpenGLIndexBuffer.h"
#include "Renderer/Renderer.h"

std::shared_ptr<IIndexBuffer> IIndexBuffer::Create(const void* Data, uint64 Size)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLIndexBuffer>(Data, Size);
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create index buffer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create index buffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create index buffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create index buffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create index buffer! Unknown/unsupported graphics API!")
    return nullptr;
}
