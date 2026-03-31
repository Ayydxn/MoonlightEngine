#include "MoonlightPCH.h"
#include "EditorTheme.h"

#include <imgui_internal.h>

void CEditorTheme::ApplyTheme()
{
    ImGuiStyle& Style = ImGui::GetStyle();

    // Rounding
    Style.WindowRounding = 0.0f;
    Style.ChildRounding = 0.0f;
    Style.FrameRounding = 2.0f;
    Style.GrabRounding = 2.0f;
    Style.PopupRounding = 2.0f;
    Style.ScrollbarRounding = 2.0f;
    Style.TabRounding = 2.0f;

    // Borders
    Style.WindowBorderSize = 1.0f;
    Style.ChildBorderSize = 1.0f;
    Style.FrameBorderSize = 0.0f;
    Style.PopupBorderSize = 1.0f;
    Style.TabBorderSize = 0.0f;

    // Spacing
    Style.WindowPadding = ImVec2(8.0f, 8.0f);
    Style.FramePadding = ImVec2(6.0f, 3.0f);
    Style.ItemSpacing = ImVec2(8.0f, 4.0f);
    Style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    Style.IndentSpacing = 16.0f;
    Style.ScrollbarSize = 10.0f;
    Style.GrabMinSize = 8.0f;
    Style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    Style.SeparatorTextPadding = ImVec2(20.0f, 3.0f);

    // Colors
    ImVec4* Colors = Style.Colors;
    Colors[ImGuiCol_Text] = CEditorThemeColors::Text;
    Colors[ImGuiCol_TextDisabled] = CEditorThemeColors::TextDisabled;
    Colors[ImGuiCol_WindowBg] = CEditorThemeColors::Background;
    Colors[ImGuiCol_ChildBg] = CEditorThemeColors::BackgroundDark;
    Colors[ImGuiCol_PopupBg] = CEditorThemeColors::BackgroundPopup;
    Colors[ImGuiCol_Border] = CEditorThemeColors::Border;
    Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    Colors[ImGuiCol_FrameBg] = CEditorThemeColors::FrameBg;
    Colors[ImGuiCol_FrameBgHovered] = CEditorThemeColors::FrameBgHovered;
    Colors[ImGuiCol_FrameBgActive] = CEditorThemeColors::FrameBgActive;
    Colors[ImGuiCol_TitleBg] = CEditorThemeColors::TitleBar;
    Colors[ImGuiCol_TitleBgActive] = CEditorThemeColors::TitleBarActive;
    Colors[ImGuiCol_TitleBgCollapsed] = CEditorThemeColors::TitleBar;
    Colors[ImGuiCol_MenuBarBg] = CEditorThemeColors::BackgroundDark;
    Colors[ImGuiCol_ScrollbarBg] = CEditorThemeColors::ScrollbarBg;
    Colors[ImGuiCol_ScrollbarGrab] = CEditorThemeColors::ScrollbarGrab;
    Colors[ImGuiCol_ScrollbarGrabHovered] = CEditorThemeColors::ScrollbarGrabHover;
    Colors[ImGuiCol_ScrollbarGrabActive] = CEditorThemeColors::ScrollbarGrabActive;
    Colors[ImGuiCol_CheckMark] = CEditorThemeColors::CheckMark;
    Colors[ImGuiCol_SliderGrab] = CEditorThemeColors::SliderGrab;
    Colors[ImGuiCol_SliderGrabActive] = CEditorThemeColors::SliderGrabActive;
    Colors[ImGuiCol_Button] = CEditorThemeColors::Button;
    Colors[ImGuiCol_ButtonHovered] = CEditorThemeColors::ButtonHovered;
    Colors[ImGuiCol_ButtonActive] = CEditorThemeColors::ButtonActive;
    Colors[ImGuiCol_Header] = CEditorThemeColors::Header;
    Colors[ImGuiCol_HeaderHovered] = CEditorThemeColors::HeaderHovered;
    Colors[ImGuiCol_HeaderActive] = CEditorThemeColors::HeaderActive;
    Colors[ImGuiCol_Separator] = CEditorThemeColors::Separator;
    Colors[ImGuiCol_SeparatorHovered] = CEditorThemeColors::Accent;
    Colors[ImGuiCol_SeparatorActive] = CEditorThemeColors::AccentActive;
    Colors[ImGuiCol_ResizeGrip] = CEditorThemeColors::ResizeGrip;
    Colors[ImGuiCol_ResizeGripHovered] = CEditorThemeColors::ResizeGripHovered;
    Colors[ImGuiCol_ResizeGripActive] = CEditorThemeColors::ResizeGripActive;
    Colors[ImGuiCol_Tab] = CEditorThemeColors::Tab;
    Colors[ImGuiCol_TabHovered] = CEditorThemeColors::TabHovered;
    Colors[ImGuiCol_TabActive] = CEditorThemeColors::TabActive;
    Colors[ImGuiCol_TabUnfocused] = CEditorThemeColors::TabUnfocused;
    Colors[ImGuiCol_TabUnfocusedActive] = CEditorThemeColors::TabUnfocusedActive;
    Colors[ImGuiCol_TabSelectedOverline] = CEditorThemeColors::TabActive;
    Colors[ImGuiCol_DockingPreview] = CEditorThemeColors::Accent;
    Colors[ImGuiCol_DockingEmptyBg] = CEditorThemeColors::BackgroundDark;
    Colors[ImGuiCol_PlotLines] = CEditorThemeColors::Accent;
    Colors[ImGuiCol_PlotLinesHovered] = CEditorThemeColors::AccentHovered;
    Colors[ImGuiCol_PlotHistogram] = CEditorThemeColors::Accent;
    Colors[ImGuiCol_PlotHistogramHovered] = CEditorThemeColors::AccentHovered;
    Colors[ImGuiCol_TableHeaderBg] = CEditorThemeColors::BackgroundDark;
    Colors[ImGuiCol_TableBorderStrong] = CEditorThemeColors::Border;
    Colors[ImGuiCol_TableBorderLight] = CEditorThemeColors::Separator;
    Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03f);
    Colors[ImGuiCol_TextSelectedBg] = ImVec4(CEditorThemeColors::Accent.x, CEditorThemeColors::Accent.y, CEditorThemeColors::Accent.z, 0.35f);
    Colors[ImGuiCol_DragDropTarget] = CEditorThemeColors::Accent;
    Colors[ImGuiCol_NavHighlight] = CEditorThemeColors::Accent;
    Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.70f);
    Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.20f);
    Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.50f);
}
