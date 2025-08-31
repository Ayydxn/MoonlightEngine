#include "MoonlightPCH.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "RHICore/ShaderCompiler.h"

void CRenderer::PreInitialize()
{
    m_RendererConfig = CRendererConfig();
    
    // TODO: (Ayydan) The graphics API we use should come from a configuration file.
    // But for now, we'll force OpenGL since it's the only one we support.
    m_GraphicsAPI = EGraphicsAPI::OpenGL;

    ENGINE_LOG_INFO_TAG("Renderer", "Selected Graphics API: {}", GetGraphicsAPIString());
}

void CRenderer::Initialize()
{
    IShaderCompiler::Init();
    
    m_RendererBackend = IRendererBackend::Create();
    m_RendererBackend->Initialize();

    m_ShaderLibrary = std::make_shared<CShaderLibrary>();
    m_ShaderLibrary->Load("Resources/Shaders/Renderer2DQuad");

    m_SceneData = new CSceneData();
}

void CRenderer::Shutdown()
{
    IShaderCompiler::GetInstance().Shutdown();

    m_RendererBackend->Shutdown();

    delete m_SceneData;
}

void CRenderer::BeginFrame()
{
    m_RendererBackend->BeginFrame();
}

void CRenderer::EndFrame()
{
    m_RendererBackend->EndFrame();
}

void CRenderer::BeginScene(const COrthographicCamera& Camera)
{
    m_SceneData->ViewProjectionMatrix = Camera.GetViewProjectionMatrix();
}

void CRenderer::EndScene()
{
}

void CRenderer::DrawIndexed(const CRenderPacket& RenderPacket, const glm::mat4& Transform, uint32 IndexCount)
{
    m_RendererBackend->DrawIndexed(RenderPacket, Transform, IndexCount, m_SceneData);
}

std::string CRenderer::GetGraphicsAPIString()
{
    switch (m_GraphicsAPI)
    {
        case EGraphicsAPI::OpenGL: return "OpenGL";
        case EGraphicsAPI::Vulkan: return "Vulkan";
        case EGraphicsAPI::Direct3D11: return "DirectX 11";
        case EGraphicsAPI::Direct3D12: return "DirectX 12";
        case EGraphicsAPI::Metal: return "Metal";
    }

    verifyEnginef(false, "Failed to get string for unknown/unsupported graphics API")
    return "Unknown Graphics API";
}
