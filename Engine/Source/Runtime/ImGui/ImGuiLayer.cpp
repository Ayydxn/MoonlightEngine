#include "MoonlightPCH.h"
#include "ImGuiLayer.h"
#include "OpenGLRHI/OpenGLImGuiLayer.h"
#include "Renderer/Renderer.h"

IImGuiLayer::IImGuiLayer()
    : CLayer("ImGui Layer") {}

IImGuiLayer* IImGuiLayer::Create()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return new COpenGLImGuiLayer();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create ImGui layer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create ImGui layer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create ImGui layer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create ImGui layer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create ImGui layer! Unknown/unsupported graphics API!")
    return nullptr;
}
