#include "MoonlightPCH.h"
#include "Renderer.h"
#include "ShaderLibrary.h"

void FRenderer::PreInitialize()
{
    // TODO: (Ayydan) The graphics API we use should come from a configuration file.
    // But for now, we'll force OpenGL since it's the only one we support.
    m_GraphicsAPI = EGraphicsAPI::OpenGL;
}

void FRenderer::Initialize()
{
    m_RHIBackend = FRHIBackend::Create();
    m_RHIBackend->Initialize();

    FShaderLibrary::Initialize();
    FShaderLibrary::GetInstance().Load("Resources/Shaders/DefaultShader.glsl");
}

void FRenderer::Shutdown()
{
    FShaderLibrary::GetInstance().Shutdown();
    
    m_RHIBackend->Shutdown();
}

void FRenderer::BeginFrame()
{
    m_RHIBackend->BeginFrame();
}

void FRenderer::EndFrame()
{
    m_RHIBackend->EndFrame();
}

void FRenderer::DrawIndexedPrimitive(const FRHICommandPacket& RHICommandPacket)
{
    m_RHIBackend->DrawIndexedPrimitive(RHICommandPacket);
}

void FRenderer::ClearColor(const float Red, const float Green, const float Blue, const float Alpha)
{
    m_RHIBackend->ClearColor(Red, Green, Blue, Alpha);
}
