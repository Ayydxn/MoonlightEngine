#include "MoonlightPCH.h"
#include "GraphicsPipeline.h"
#include "OpenGLRHI/OpenGLGraphicsPipeline.h"
#include "Renderer/Renderer.h"

std::shared_ptr<FGraphicsPipeline> FGraphicsPipeline::Create(const FGraphicsPipelineSpecification& GraphicsPipelineSpecification)
{
    m_Specification = GraphicsPipelineSpecification;
    
    switch (FRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return std::make_shared<FOpenGLGraphicsPipeline>();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create graphics pipeline! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create graphics pipeline! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create graphics pipeline! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create graphics pipeline! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create graphics pipeline! Unknown/unsupported graphics API!")
    return nullptr;
}
