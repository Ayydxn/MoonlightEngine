#include "MoonlightPCH.h"
#include "UIComponents.h"

#include <imgui.h>
#include <imgui_internal.h>

void CUIComponents::Vector3Control(const std::string& Name, glm::vec3& Values, float ResetValue, float ColumnWidth)
{
    ImGui::PushID(Name.c_str());
    
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, ColumnWidth);
    
    ImGui::Text("%s", Name.c_str());
    
    ImGui::NextColumn();
    
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });
    
    /* -- X -- */
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.886f, 0.161f, 0.0f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.950f, 0.250f, 0.100f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.700f, 0.120f, 0.000f, 1.0f });
    
    const float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    const ImVec2 ButtonSize = { LineHeight + 3.0f, LineHeight };
    
    if (ImGui::Button("X", ButtonSize))
        Values.x = ResetValue;
    
    ImGui::SameLine();
    ImGui::DragFloat("##X", &Values.x, 0.1f);
    
    ImGui::PopStyleColor(3);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    
    /* -- Y -- */
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.443f, 0.733f, 0.0f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.520f, 0.850f, 0.000f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.350f, 0.580f, 0.000f, 1.0f });
    
    if (ImGui::Button("Y", ButtonSize))
        Values.y = ResetValue;
    
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &Values.y, 0.1f);
    
    ImGui::PopStyleColor(3);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    
    /* -- Z -- */
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.180f, 0.525f, 0.992f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.300f, 0.620f, 1.000f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.140f, 0.410f, 0.780f, 1.0f });
    
    if (ImGui::Button("Z", ButtonSize))
        Values.z = ResetValue;
    
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &Values.z, 0.1f);
    
    ImGui::PopStyleColor(3);
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    
    ImGui::PopID();
}

void CUIComponents::CenteredText(const std::string& Text, float Opacity)
{
    const float TextWidth = ImGui::CalcTextSize(Text.c_str()).x;
    const float WindowWidth = ImGui::GetContentRegionAvail().x;
    const float TextIndentation = (WindowWidth - TextWidth) * 0.5f;
    
    // (Ayydxn) May need to add a check for minimum indentation if the text is too long?
    if (TextIndentation > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + TextIndentation);
    
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Opacity);
    
    ImGui::Text("%s", Text.c_str());
    
    ImGui::PopStyleVar();
}
