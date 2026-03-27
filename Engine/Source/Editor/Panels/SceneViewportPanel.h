#pragma once

#include "CoreDefines.h"
#include "Editor/EditorViewportCamera.h"
#include "Renderer/SceneRenderer.h"
#include "RHICore/Framebuffer.h"
#include "Scene/Scene.h"

#include <glm/vec2.hpp>

class MOONLIGHT_API CSceneViewportPanel
{
public:
    CSceneViewportPanel() = default;
    CSceneViewportPanel(const std::shared_ptr<CSceneRenderer>& SceneRenderer, const std::shared_ptr<CScene>& Scene);
    
    void OnUpdate(float DeltaTime);
    void OnEvent(IEvent& Event);
    void OnRender() const;
    void OnImGuiRender();
    
    void SetActiveScene(const std::shared_ptr<CScene>& Scene) { m_ActiveScene = Scene; }
private:
    std::shared_ptr<CSceneRenderer> m_SceneRenderer;
    std::shared_ptr<CScene> m_ActiveScene;
    std::shared_ptr<IFramebuffer> m_Framebuffer;
    
    CEditorViewportCamera m_Camera;
    glm::vec2 m_Size;
    
    bool bIsFocused = false;
};
