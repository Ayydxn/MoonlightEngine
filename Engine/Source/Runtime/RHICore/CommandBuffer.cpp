#include "MoonlightPCH.h"
#include "CommandBuffer.h"
#include "Renderer/Renderer.h"
#include "VulkanRHI/VulkanCommandBuffer.h"

std::shared_ptr<ICommandBuffer> ICommandBuffer::Create(uint32 Count)
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: verifyEnginef(false, "Failed to create command buffer! OpenGL isn't supported!") return nullptr;
        case EGraphicsAPI::Vulkan: return std::make_shared<CVulkanCommandBuffer>(Count);
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create command buffer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create command buffer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create command buffer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create command buffer! Unknown/unsupported graphics API!")
    return nullptr;
}
