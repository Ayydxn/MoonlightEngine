#include "MoonlightPCH.h"
#include "SceneRenderer.h"

CSceneRenderer::CSceneRenderer()
{
    Initialize();
}

CSceneRenderer::~CSceneRenderer()
{
    Shutdown();
}

void CSceneRenderer::Initialize()
{
    m_Renderer2D = std::make_shared<CRenderer2D>();
}

void CSceneRenderer::Shutdown()
{
}
