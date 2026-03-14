#include "MoonlightPCH.h"
#include "CommandList.h"
#include "Renderer/Renderer.h"
#include "OpenGLRHI/OpenGLCommandList.h"

std::shared_ptr<ICommandList> ICommandList::Create()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<COpenGLCommandList>();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create command list! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create command list! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create command list! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create command list! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create command list! Unknown/unsupported graphics API!")
    return nullptr;
}
