#pragma once

#include "Layers/Layer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Camera/OrthographicCameraController.h"
#include "RHICore/Framebuffer.h"
#include "Scene/Scene.h"

#include <imgui.h>

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer();
    
    void OnAttach() override;
    void OnUpdate(float DeltaTime) override;
    void OnEvent(IEvent& Event) override;
    void OnRender() override;
    void OnImGuiRender() override;
private:
    void UI_RenderDockSpace();
    void UI_RenderMenuBar();
    void UI_RenderViewport();
private:
    std::shared_ptr<CSceneRenderer> m_SceneRenderer;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
    std::shared_ptr<ITexture> m_MushroomTexture;
    std::shared_ptr<IFramebuffer> m_SceneFramebuffer;
    std::shared_ptr<CScene> m_ActiveScene;
    
    CEditorViewportCamera m_ViewportCamera;
    ImVec2 m_ViewportSize = { 0.0f, 0.0f };
    
    float m_QuadRotation = 0.0f;
};
