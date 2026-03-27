#pragma once

#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/Panels/SceneViewportPanel.h"
#include "Layers/Layer.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"

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
private:
    std::shared_ptr<CSceneRenderer> m_SceneRenderer;
    std::shared_ptr<CScene> m_ActiveScene;
    
    std::unique_ptr<CSceneViewportPanel> m_SceneViewportPanel;
    std::unique_ptr<CSceneHierarchyPanel> m_SceneHierarchyPanel;
};
