#include "MoonlightPCH.h"
#include "ImGuiLayer.h"
#include "OpenGLRHI/OpenGLImGuiLayer.h"
#include "Renderer/Renderer.h"

#include <imgui.h>

IImGuiLayer::IImGuiLayer()
    : CLayer("ImGui Layer") {}


IImGuiLayer* IImGuiLayer::Create()
{
    switch (CRenderer::GetGraphicsAPI())
    {
        case EGraphicsAPI::OpenGL: return new COpenGLImGuiLayer();
        case EGraphicsAPI::Vulkan: verifyEnginef(false, "Failed to create ImGui layer! Vulkan isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D11: verifyEnginef(false, "Failed to create ImGui layer! DirectX 11 isn't supported!") return nullptr;
        case EGraphicsAPI::Direct3D12: verifyEnginef(false, "Failed to create ImGui layer! DirectX 12 isn't supported!") return nullptr;
        case EGraphicsAPI::Metal: verifyEnginef(false, "Failed to create ImGui layer! Metal isn't supported!") return nullptr;
    }

    verifyEnginef(false, "Failed to create ImGui layer! Unknown/unsupported graphics API!")
    return nullptr;
}

// NOTE: Taken from Hazel Engine by TheCherno.
void IImGuiLayer::SetDarkThemeColors()
{
	auto& Style = ImGui::GetStyle();
	auto& Colors = ImGui::GetStyle().Colors;

	/*--------------*/
	/* -- Colors -- */
	/*--------------*/

	// Headers
	Colors[ImGuiCol_Header] = ImGui::ColorConvertU32ToFloat4(IM_COL32(47, 47, 47, 255));
	Colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(IM_COL32(105, 105, 105, 255));
	Colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(47, 47, 47, 255));

	// Buttons
	Colors[ImGuiCol_Button] = ImColor(56, 56, 56, 200);
	Colors[ImGuiCol_ButtonHovered] = ImColor(105, 105, 105, 255);
	Colors[ImGuiCol_ButtonActive] = ImColor(56, 56, 56, 150);

	// Frame BG
	Colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(15, 15, 15, 255));
	Colors[ImGuiCol_FrameBgHovered] = ImGui::ColorConvertU32ToFloat4(IM_COL32(105, 105, 105, 255));
	Colors[ImGuiCol_FrameBgActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(15, 15, 15, 255));

	// Tabs
	Colors[ImGuiCol_Tab] = ImGui::ColorConvertU32ToFloat4(IM_COL32(21, 21, 21, 255));
	Colors[ImGuiCol_TabHovered] = ImColor(105, 105, 105, 255);
	Colors[ImGuiCol_TabActive] = ImColor(255, 225, 135, 60);
	Colors[ImGuiCol_TabUnfocused] = ImGui::ColorConvertU32ToFloat4(IM_COL32(21, 21, 21, 255));
	Colors[ImGuiCol_TabUnfocusedActive] = Colors[ImGuiCol_TabHovered];

	// Title
	Colors[ImGuiCol_TitleBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(21, 21, 21, 255));
	Colors[ImGuiCol_TitleBgActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(21, 21, 21, 255));
	Colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

	// Resize Grip
	Colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

	// Scrollbar
	Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
	Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
	Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

	// Check Mark
	Colors[ImGuiCol_CheckMark] = ImColor(200, 200, 200, 255);

	// Slider
	Colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
	Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

	// Text
	Colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(IM_COL32(192, 192, 192, 255));

	// Checkbox
	Colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(IM_COL32(192, 192, 192, 255));

	// Separator
	Colors[ImGuiCol_Separator] = ImGui::ColorConvertU32ToFloat4(IM_COL32(26, 26, 26, 255));
	Colors[ImGuiCol_SeparatorActive] = ImGui::ColorConvertU32ToFloat4(IM_COL32(39, 185, 242, 255));
	Colors[ImGuiCol_SeparatorHovered] = ImColor(39, 185, 242, 150);

	// Window Background
	Colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(21, 21, 21, 255));
	Colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(36, 36, 36, 255));
	Colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(50, 50, 50, 255));
	Colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(IM_COL32(26, 26, 26, 255));

	// Tables
	Colors[ImGuiCol_TableHeaderBg] = ImGui::ColorConvertU32ToFloat4(IM_COL32(47, 47, 47, 255));
	Colors[ImGuiCol_TableBorderLight] = ImGui::ColorConvertU32ToFloat4(IM_COL32(26, 26, 26, 255));

	// Menubar
	Colors[ImGuiCol_MenuBarBg] = ImVec4{0.0f, 0.0f, 0.0f, 0.0f};

	/*-------------*/
	/* -- Style -- */
	/*-------------*/
	
	Style.FrameRounding = 2.5f;
	Style.FrameBorderSize = 1.0f;
	Style.IndentSpacing = 11.0f;
}
