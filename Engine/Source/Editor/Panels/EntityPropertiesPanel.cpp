#include "MoonlightPCH.h"
#include "EntityPropertiesPanel.h"
#include "ImGui/UIComponents.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/NameComponent.h"
#include "Scene/Components/SpriteRendererComponent.h"
#include "Scene/Components/TagComponent.h"
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
    
    ImGui::Separator();
    
    DrawAddComponentMenu();
    
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
    }, {.bNestInTreeNode = false, .bIsRemovable = false, });
    
    DrawComponent<CTransformComponent>("Transform", [](auto& TransformComponent)
    {
        CUIComponents::Vector3Control("Position", TransformComponent.Position);
        
        // Convert the rotation to degrees so we can display it as such in the UI,
        // but convert it back to radians after since that is what the engine expects to deal with. 
        glm::vec3 RotationDegrees = glm::degrees(TransformComponent.Rotation);
        CUIComponents::Vector3Control("Rotation", RotationDegrees);
        TransformComponent.Rotation = glm::radians(RotationDegrees);
        
        CUIComponents::Vector3Control("Scale", TransformComponent.Scale, 1.0f);
    }, { .bIsRemovable = false });
    
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
    
    DrawComponent<CTagComponent>("Tags", [this](auto& TagComponent)
    {
        auto TryAddTag = [&]()
        {
            if (strlen(m_NewTagBuffer) == 0)
                return;

            if (TagComponent.HasTag(m_NewTagBuffer))
            {
                std::memcpy(m_DuplicateTagName, m_NewTagBuffer, sizeof(m_DuplicateTagName));
                bOpenDuplicatePopup = true;
            }
            else
            {
                TagComponent.AddTag(m_NewTagBuffer);
                std::memset(m_NewTagBuffer, 0, sizeof(m_NewTagBuffer));
            }
        };

        if (ImGui::InputTextWithHint("##AddTag", "Enter Tag Name...", m_NewTagBuffer, sizeof(m_NewTagBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            TryAddTag();

        ImGui::SameLine();

        if (ImGui::Button("Add"))
            TryAddTag();

        // Popup open is deferred by one frame to avoid ID stack issues
        if (bOpenDuplicatePopup)
        {
            ImGui::OpenPopup("Duplicate Tag");
            bOpenDuplicatePopup = false;
        }

        if (ImGui::BeginPopupModal("Duplicate Tag", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("The tag \"%s\" already exists for this entity!", m_DuplicateTagName);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (CUIComponents::CenteredButton("OK", { 120.0f, 0.0f }))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }

        ImGui::Spacing();

        if (ImGui::BeginListBox("##TagsList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            if (TagComponent.Tags.empty())
            {
                CUIComponents::CenteredText("No Tags Assigned...", 0.75f);
            }
            else
            {
                std::string TagToDelete;
    
                for (const auto& Tag : TagComponent.Tags)
                {
                    ImGui::PushID(Tag.c_str());
                    ImGui::Selectable(Tag.c_str(), false);
    
                    if (ImGui::BeginPopupContextItem())
                    {
                        if (ImGui::MenuItem("Delete Tag"))
                            TagToDelete = Tag;
                        
                        ImGui::EndPopup();
                    }
    
                    ImGui::PopID();
                }
    
                if (!TagToDelete.empty())
                    TagComponent.RemoveTag(TagToDelete);
            }

            ImGui::EndListBox();
        }

        ImGui::TextDisabled("(Right-click a tag to delete it)");
    });
}

void CEntityPropertiesPanel::DrawAddComponentMenu()
{
    if (CUIComponents::CenteredButton("Add Component", { 200.0f, 0.0f }))
        ImGui::OpenPopup("AvailableComponents");
    
    if (ImGui::BeginPopup("AvailableComponents"))
    {
        if (ImGui::MenuItem("Camera"))
        {
            m_Entity.AddComponent<CCameraComponent>();
            
            ImGui::CloseCurrentPopup();
        }
        
        if (ImGui::MenuItem("Sprite Renderer"))
        {
            m_Entity.AddComponent<CSpriteRendererComponent>();
            
            ImGui::CloseCurrentPopup();
        }
        
        if (ImGui::MenuItem("Tags"))
        {
            m_Entity.AddComponent<CTagComponent>();
            
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
}

template<typename T, typename DrawFunction>
void CEntityPropertiesPanel::DrawComponent(const std::string& Name, DrawFunction UIDrawFunction, CComponentDrawOptions DrawOptions)
{
    constexpr ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    
    if (m_Entity.HasComponent<T>())
    {
        if (DrawOptions.bNestInTreeNode)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { 4, 4 });
            ImGui::Separator();
            
            const bool bIsTreeNodeOpen = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), TreeNodeFlags, "%s", Name.c_str());
            
            ImGui::PopStyleVar();
            
            bool bWasMarkedForDeletion = false;
            
            if (DrawOptions.bIsRemovable && ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Remove"))
                    bWasMarkedForDeletion = true;
                
                ImGui::EndPopup();
            }
        
            if (bIsTreeNodeOpen)
            {
                UIDrawFunction(m_Entity.GetComponent<T>());
            
                ImGui::TreePop();
                
                if (bWasMarkedForDeletion)
                    m_Entity.RemoveComponent<T>();
            }
            
            return;
        }
        
        // We don't want to nest the component's UI in a tree node, so just call the draw function.
        UIDrawFunction(m_Entity.GetComponent<T>());
    }
}
