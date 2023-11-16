#include "MoonlightPCH.h"
#include "Renderer.h"

void FRenderer::PreInitialize()
{
    // TODO: (Ayydan) The graphics API we use should come from a configuration file.
    // But for now, we'll force OpenGL since it's the only one we support.
    m_GraphicsAPI = EGraphicsAPI::OpenGL;
}

void FRenderer::Initialize()
{
}

void FRenderer::Shutdown()
{
}

void FRenderer::BeginFrame()
{
}

void FRenderer::EndFrame()
{
}