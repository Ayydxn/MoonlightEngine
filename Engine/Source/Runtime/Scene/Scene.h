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
    
    CEntity CreateEntity();
private:
    entt::registry m_EntityRegistry;
    
    friend class CEntity;
};
