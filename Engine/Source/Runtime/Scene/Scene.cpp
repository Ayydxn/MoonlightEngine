#include "MoonlightPCH.h"
#include "Scene.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Entity/Entity.h"

void CScene::OnUpdate(float DeltaTime)
{
}

void CScene::OnFixedUpdate(float FixedDeltaTime)
{
}

void CScene::OnRenderEditor(const std::shared_ptr<CSceneRenderer>& SceneRenderer, const CEditorViewportCamera& ViewportCamera)
{
    const auto Renderer2D = SceneRenderer->GetRenderer2D();
    Renderer2D->ResetStats();
    Renderer2D->BeginFrame(ViewportCamera);
    
    /* -- Render sprites -- */
    {
        for (const auto Group = m_EntityRegistry.group<CTransformComponent>(entt::get<CSpriteRendererComponent>); const auto& Entity : Group)
        {
            const auto& [Transform, Sprite] = Group.get<CTransformComponent, CSpriteRendererComponent>(Entity);

            Renderer2D->DrawQuad(Transform.GetTransformMatrix(), Sprite.Color);
        }
    }
    
    Renderer2D->EndFrame();
}

CEntity CScene::CreateEntity()
{
    CEntity Entity = { m_EntityRegistry.create(), this };
    Entity.AddComponent<CTransformComponent>();
    
    return Entity;
}
