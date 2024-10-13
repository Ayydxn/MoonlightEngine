#pragma once

#include "Application/Application.h"
#include "RHICore/RendererBackend.h"
#include "RHICore/RendererContext.h"

enum class EGraphicsAPI
{
    OpenGL,
    Vulkan,
    Direct3D11,
    Direct3D12,
    Metal
};

class CRenderer
{
public:
    static void PreInitialize();
    static void Initialize();
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();

    static void DrawIndexed(const CRenderPacket& RenderPacket);
    
    static std::shared_ptr<IRendererContext> GetContext() { return CApplication::GetInstance().GetWindow().GetRendererContext(); }
    
    static EGraphicsAPI GetGraphicsAPI() { return m_GraphicsAPI; }
    static std::string GetGraphicsAPIString();
    static void SetGraphicsAPI(EGraphicsAPI NewGraphicsAPI) { m_GraphicsAPI = NewGraphicsAPI; }
private:
    inline static std::shared_ptr<IRendererBackend> m_RendererBackend;
    
    inline static EGraphicsAPI m_GraphicsAPI;
};
