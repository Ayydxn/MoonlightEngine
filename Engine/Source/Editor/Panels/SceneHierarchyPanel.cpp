#include "MoonlightPCH.h"
#include "SceneHierarchyPanel.h"
#include "Scene/Components/NameComponent.h"

#include <imgui.h>

CSceneHierarchyPanel::CSceneHierarchyPanel(const std::shared_ptr<CScene>& Scene)
    : m_Scene(Scene) {}

void CSceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");
    
    for (const auto& Entity : m_Scene->m_EntityRegistry.view<entt::entity>())
        DrawEntityNode({ Entity, m_Scene.get() });
    
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        m_SelectedEntity = {};
        
        m_PropertiesPanel.ClearEntity();
    }
    
    if (ImGui::BeginPopupContextWindow("##HierarchyContextWindow", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        if (ImGui::MenuItem("New Entity"))
            m_Scene->CreateEntity("Entity");
        
        ImGui::EndPopup();
    }
    
    ImGui::End();
    
    m_PropertiesPanel.OnImGuiRender();
}

void CSceneHierarchyPanel::DrawEntityNode(CEntity Entity)
{
    const auto& EntityName = Entity.GetComponent<CNameComponent>();
    ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_OpenOnArrow;
    
    if (m_SelectedEntity == Entity)
        Flags |= ImGuiTreeNodeFlags_Selected;
    
    bool bIsParentNodeOpened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64>(static_cast<uint32>(Entity))), Flags, "%s",
        EntityName.Name.c_str());
    
    bool bMarkedForDeletion = false;
    
    if (ImGui::IsItemClicked())
    {
        m_SelectedEntity = Entity;
        
        m_PropertiesPanel.SetEntity(m_SelectedEntity);
    }
    
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete"))
            bMarkedForDeletion = true;
        
        ImGui::EndPopup();
    }
    
    // TODO: (Ayydxn) In the future, display child entities. 
    if (bIsParentNodeOpened)
        ImGui::TreePop();
    
    if (bMarkedForDeletion)
    {
        m_Scene->DestroyEntity(Entity);
        
        if (m_SelectedEntity == Entity)
            m_SelectedEntity = {};
    }
}
