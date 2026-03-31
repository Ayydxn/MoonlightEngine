#include "MoonlightPCH.h"
#include "ImGuiLayer.h"
#include "ImGuiFonts.h"
#include "Editor/EditorTheme.h"
#include "OpenGLRHI/OpenGLImGuiLayer.h"
#include "Renderer/Renderer.h"

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

void IImGuiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	IO.ConfigWindowsMoveFromTitleBarOnly = true;
	
	// Setup fonts
	CImGuiFonts::Add({
		.Name = "Inter",
		.Filepath = "Resources/Fonts/Inter_28pt-Regular.ttf",
		.Size = 17.0f
	}, true);
	
	CImGuiFonts::Add({
		.Name = "Inter-Bold",
		.Filepath = "Resources/Fonts/Inter_28pt-Bold.ttf",
		.Size = 17.0f
	});
	
	CImGuiFonts::Add({
		.Name = "Inter-Italic",
		.Filepath = "Resources/Fonts/Inter_28pt-Italic.ttf",
		.Size = 17.0f
	});
	
	// Setup styling/theming
	ImGui::StyleColorsDark();
	CEditorTheme::ApplyTheme();

	ImGuiStyle& Style = ImGui::GetStyle();
	if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		Style.WindowRounding = 0.0f;
		Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
}

void IImGuiLayer::OnEvent(IEvent& Event)
{
	if (bBlockingEvents)
	{
		const ImGuiIO& IO = ImGui::GetIO();
		
		Event.bIsHandled |= Event.IsInCategory(KeyboardCategory) & IO.WantCaptureKeyboard;
		Event.bIsHandled |= Event.IsInCategory(MouseCategory) & IO.WantCaptureMouse;
	}
}
