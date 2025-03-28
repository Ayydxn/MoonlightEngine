﻿#pragma once

#include "CoreDefines.h"
#include "RendererConfig.h"
#include "Application/Application.h"
#include "Camera/OrthographicCamera.h"
#include "RHICore/RendererBackend.h"
#include "RHICore/RendererContext.h"
#include "Shaders/ShaderLibrary.h"

enum class EGraphicsAPI
{
    OpenGL,
    Vulkan,
    Direct3D11,
    Direct3D12,
    Metal
};

struct CSceneData
{
    glm::mat4 ViewProjectionMatrix;
};

class MOONLIGHT_API CRenderer
{
public:
    static void PreInitialize();
    static void Initialize();
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();

    static void BeginScene(const COrthographicCamera& Camera);
    static void EndScene();

    static void DrawIndexed(const CRenderPacket& RenderPacket, const glm::mat4& Transform = glm::mat4(1.0f));
    
    static std::shared_ptr<IRendererContext> GetContext() { return CApplication::GetInstance().GetWindow().GetRendererContext(); }
    static std::shared_ptr<CShaderLibrary> GetShaderLibrary() { return m_ShaderLibrary; }
    static CRendererConfig GetConfig() { return m_RendererConfig; }
    
    static EGraphicsAPI GetGraphicsAPI() { return m_GraphicsAPI; }
    static std::string GetGraphicsAPIString();
    static void SetGraphicsAPI(EGraphicsAPI NewGraphicsAPI) { m_GraphicsAPI = NewGraphicsAPI; }
protected:
    inline static CSceneData* m_SceneData;
private:
    inline static std::shared_ptr<IRendererBackend> m_RendererBackend;
    inline static std::shared_ptr<CShaderLibrary> m_ShaderLibrary;

    inline static CRendererConfig m_RendererConfig;
    
    inline static EGraphicsAPI m_GraphicsAPI;
};
