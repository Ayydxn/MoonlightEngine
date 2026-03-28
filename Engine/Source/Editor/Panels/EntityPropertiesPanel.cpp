#include "MoonlightPCH.h"
#include "EntityPropertiesPanel.h"
#include "ImGui/UIComponents.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/NameComponent.h"
#include "Scene/Components/SpriteRendererComponent.h"
#include "Scene/Components/TransformComponent.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

CEntityPropertiesPanel::CEntityPropertiesPanel(CEntity Entity)
    : m_Entity(Entity) {}

void CEntityPropertiesPanel::OnImGuiRender()
{
    ImGui::Begin("Properties");
    
    if (!m_Entity)
    {
        CUIComponents::CenteredText("Select an entity to view its properties", 0.75f);
        
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
    }, false);
    
    DrawComponent<CTransformComponent>("Transform", [](auto& TransformComponent)
    {
        CUIComponents::Vector3Control("Position", TransformComponent.Position);
        
        // Convert the rotation to degrees so we can display it as such in the UI,
        // but convert it back to radians after since that is what the engine expects to deal with. 
        glm::vec3 RotationDegrees = glm::degrees(TransformComponent.Rotation);
        CUIComponents::Vector3Control("Rotation", RotationDegrees);
        TransformComponent.Rotation = glm::radians(RotationDegrees);
        
        CUIComponents::Vector3Control("Scale", TransformComponent.Scale, 1.0f);
    });
    
    DrawComponent<CCameraComponent>("Camera", [](auto& CameraComponent)
    {
        const char* ProjectionTypes[] = { "Perspective", "Orthographic" };
        const char* CurrentProjectionType = ProjectionTypes[static_cast<int>(CameraComponent.Camera.GetProjectionType())];
        
        if (ImGui::BeginCombo("Projection Type", CurrentProjectionType))
        {
            for (int i = 0; i < IM_ARRAYSIZE(ProjectionTypes); i++)
            {
                const bool bIsSelected = CurrentProjectionType == ProjectionTypes[i];
                
                if (ImGui::Selectable(ProjectionTypes[i], bIsSelected))
                    CameraComponent.Camera.SetProjectionType(static_cast<EProjectionType>(i));
                
                if (bIsSelected)
                    ImGui::SetItemDefaultFocus();
            }
            
            ImGui::EndCombo();
        }
        
        if (CameraComponent.Camera.GetProjectionType() == EProjectionType::Perspective)
        {
            float FOV = glm::degrees(CameraComponent.Camera.GetPerspectiveFOV());
            float NearClip = CameraComponent.Camera.GetPerspectiveNearClip();
            float FarClip = CameraComponent.Camera.GetPerspectiveFarClip();
            
            if (ImGui::DragFloat("FOV", &FOV, 1.0f, 0.0f, 0.0f, "%.2f"))
                CameraComponent.Camera.SetPerspectiveFOV(glm::radians(FOV));
            
            if (ImGui::DragFloat("Near Clip", &NearClip, 1.0f, 0.0f, 0.0f, "%.2f"))
                CameraComponent.Camera.SetPerspectiveNearClip(NearClip);
            
            if (ImGui::DragFloat("Far Clip", &FarClip, 1.0f, 0.0f, 0.0f, "%.2f"))
                CameraComponent.Camera.SetPerspectiveFarClip(FarClip);
        }
        else if (CameraComponent.Camera.GetProjectionType() == EProjectionType::Orthographic)
        {
            float Size = CameraComponent.Camera.GetOrthographicSize();
            float NearClip = CameraComponent.Camera.GetOrthographicNearClip();
            float FarClip = CameraComponent.Camera.GetOrthographicFarClip();
            
            if (ImGui::DragFloat("Size", &Size, 1.0f, 0.0f, 0.0f, "%.2f"))
                CameraComponent.Camera.SetOrthographicSize(Size);
            
            if (ImGui::DragFloat("Near Clip", &NearClip, 1.0f, 0.0f, 0.0f, "%.2f"))
                CameraComponent.Camera.SetOrthographicNearClip(NearClip);
            
            if (ImGui::DragFloat("Far Clip", &FarClip, 1.0f, 0.0f, 0.0f, "%.2f"))
                CameraComponent.Camera.SetOrthographicFarClip(FarClip);
        }
        
        bool bIsPrimary = CameraComponent.bIsPrimary;
        bool bUsingFixedAspectRatio = CameraComponent.bUseFixedAspectRatio;
        
        if (ImGui::Checkbox("Primary", &bIsPrimary))
            CameraComponent.bIsPrimary = bIsPrimary;
        
        if (ImGui::Checkbox("Fixed Aspect Ratio", &bUsingFixedAspectRatio))
            CameraComponent.bUseFixedAspectRatio = bUsingFixedAspectRatio;
    });
    
    DrawComponent<CSpriteRendererComponent>("Sprite Renderer", [](auto& SpriteRendererComponent)
    {
        ImGui::ColorEdit4("Color", glm::value_ptr(SpriteRendererComponent.Color));
    });
}

template<typename T, typename DrawFunction>
void CEntityPropertiesPanel::DrawComponent(const std::string& Name, DrawFunction UIDrawFunction, bool bNestInTreeNode)
{
    constexpr ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    
    if (m_Entity.HasComponent<T>())
    {
        if (bNestInTreeNode)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { 4, 4 });
            ImGui::Separator();
            
            const bool bIsTreeNodeOpen = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), TreeNodeFlags, "%s", Name.c_str());
            ImGui::PopStyleVar();
        
            if (bIsTreeNodeOpen)
            {
                UIDrawFunction(m_Entity.GetComponent<T>());
            
                ImGui::TreePop();
            }
            
            return;
        }
        
        // We don't want to nest the component's UI in a tree node, so just call the draw function.
        UIDrawFunction(m_Entity.GetComponent<T>());
    }
}
