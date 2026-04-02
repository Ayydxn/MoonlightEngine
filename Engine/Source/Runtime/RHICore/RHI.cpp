#include "MoonlightPCH.h"
#include "RHI.h"
#include "OpenGLRHI/OpenGLRHIFactory.h"
#include "Renderer/Renderer.h"

std::unique_ptr<IRHIFactory> CRHI::m_Factory = nullptr;

void CRHI::Initialize()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: m_Factory = std::make_unique<COpenGLRHIFactory>(); break;
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create RHI factory! Vulkan isn't supported!") break;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create RHI factory! DirectX 11 isn't supported!") break;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create RHI factory! DirectX 12 isn't supported!") break;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create RHI factory! Metal isn't supported!") break;
    }
    
    ENGINE_LOG_INFO_TAG("RHI", "Successfully initialized RHI Factory for {}", CRenderer::GetGraphicsAPIString());
}

void CRHI::Shutdown()
{
    m_Factory.reset();
}

IRHIFactory& CRHI::GetFactory()
{
    verifyEnginef(m_Factory, "The RHI has not been initialized yet!")
    
    return *m_Factory;
}
