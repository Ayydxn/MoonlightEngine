#include "MoonlightPCH.h"
#include "SceneHierarchyPanel.h"
#include "Scene/Components/TagComponent.h"

#include <imgui.h>

CSceneHierarchyPanel::CSceneHierarchyPanel(const std::shared_ptr<CScene>& Scene)
    : m_Scene(Scene) {}

void CSceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");
    
    for (const auto& Entity : m_Scene->m_EntityRegistry.view<entt::entity>())
        DrawEntityNode({ Entity, m_Scene.get() });
    
    ImGui::End();
}

void CSceneHierarchyPanel::DrawEntityNode(CEntity Entity)
{
    const auto& TagComponent = Entity.GetComponent<CTagComponent>();
    ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_OpenOnArrow;
    
    if (m_SelectedEntity == Entity)
        Flags |= ImGuiTreeNodeFlags_Selected;
    
    bool bIsParentNodeOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64>(static_cast<uint32>(Entity))), Flags, "%s", TagComponent.Tag.c_str());
    
    if (ImGui::IsItemClicked())
        m_SelectedEntity = Entity;
    
    // TODO: (Ayydxn) In the future, display child entities. 
    if (bIsParentNodeOpened)
        ImGui::TreePop();
}
