#include "MoonlightPCH.h"
#include "EntityPropertiesPanel.h"
#include "Scene/Components/NameComponent.h"
#include "Scene/Components/TransformComponent.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

CEntityPropertiesPanel::CEntityPropertiesPanel(CEntity Entity)
    : m_Entity(Entity) {}

void CEntityPropertiesPanel::OnImGuiRender()
{
    ImGui::Begin("Properties");
    
    if (!m_Entity)
    {
        ImGui::Text("Select an entity to view its properties");
        ImGui::End();
        
        return;
    }
    
    DrawEntityComponents();
    
    ImGui::End();
}

void CEntityPropertiesPanel::DrawEntityComponents()
{
    DrawComponent<CNameComponent>("Name", [](auto& NameComponent)
    {
        char Buffer[256] = {};
        Buffer[0] = 0;
        std::memcpy(Buffer, NameComponent.Name.c_str(), NameComponent.Name.length());
        
        if (ImGui::InputText("Name", Buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            NameComponent.Name = std::string(Buffer);
            
            if (NameComponent.Name.empty())
                NameComponent.Name = "Entity";
        }
    });
    
    DrawComponent<CTransformComponent>("Transform", [](auto& TransformComponent)
    {
        ImGui::DragFloat3("Position", glm::value_ptr(TransformComponent.Position), 0.25f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(TransformComponent.Rotation), 0.25f);
        ImGui::DragFloat3("Scale", glm::value_ptr(TransformComponent.Scale), 0.25f);
    });
}

template<typename T, typename DrawFunction>
void CEntityPropertiesPanel::DrawComponent(const std::string& Name, DrawFunction UIDrawFunction)
{
    constexpr ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    
    if (m_Entity.HasComponent<T>())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { 4, 4 });
        ImGui::Separator();

        const bool bIsTreeNodeOpen = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), TreeNodeFlags, Name.c_str());
        ImGui::PopStyleVar();
        
        if (bIsTreeNodeOpen)
        {
            UIDrawFunction(m_Entity.GetComponent<T>());
            
            ImGui::TreePop();
        }
    }
}
