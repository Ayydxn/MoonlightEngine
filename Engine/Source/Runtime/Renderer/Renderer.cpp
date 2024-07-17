#include "MoonlightPCH.h"
#include "Renderer.h"

void CRenderer::PreInitialize()
{
    // TODO: (Ayydan) The graphics API we use should come from a configuration file.
    // But for now, we'll force Vulkan since it's the only one we support.
    m_GraphicsAPI = EGraphicsAPI::Vulkan;
}

void CRenderer::Initialize()
{
}

void CRenderer::Shutdown()
{
}

void CRenderer::BeginFrame()
{
}

void CRenderer::EndFrame()
{
}
