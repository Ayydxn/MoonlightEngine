#pragma once

#include "Renderer/SceneRenderer.h"

#include <entt/entt.hpp>

class CEntity;

class MOONLIGHT_API CScene
{
public:
    CScene() = default;
    ~CScene() = default;
    
    void OnUpdate(float DeltaTime);
    void OnFixedUpdate(float FixedDeltaTime);
    void OnRenderEditor(const std::shared_ptr<CSceneRenderer>& SceneRenderer, const CEditorViewportCamera& ViewportCamera);
    
    void OnViewportResize(uint32_t ViewportWidth, uint32_t ViewportHeight);
    
    CEntity CreateEntity();
private:
    entt::registry m_EntityRegistry;
    
    uint32 m_ViewportWidth = 0;
    uint32 m_ViewportHeight = 0;
    
    friend class CEntity;
};
