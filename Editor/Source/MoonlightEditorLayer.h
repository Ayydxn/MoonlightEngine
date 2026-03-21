#pragma once

#include "Layers/Layer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Camera/OrthographicCameraController.h"
#include "RHICore/Framebuffer.h"

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
    std::shared_ptr<CRenderer2D> m_Renderer2D;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
    std::shared_ptr<ITexture> m_MushroomTexture;
    std::shared_ptr<IFramebuffer> m_SceneFramebuffer;
    
    COrthographicCameraController m_CameraController;
    ImVec2 m_ViewportSize = { 0.0f, 0.0f };
    
    bool bIsSceneViewportFocused = false;
    float m_QuadRotation = 0.0f;
};
