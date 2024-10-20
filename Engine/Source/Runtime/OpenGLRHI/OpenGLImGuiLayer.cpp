#include "MoonlightPCH.h"
#include "OpenGLImGuiLayer.h"
#include "Application/Application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

void COpenGLImGuiLayer::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void COpenGLImGuiLayer::EndFrame()
{
    CApplication& Application = CApplication::GetInstance();
    
    ImGuiIO& IO = ImGui::GetIO();
    IO.DisplaySize = ImVec2(static_cast<float>(Application.GetWindow().GetWidth()), static_cast<float>(Application.GetWindow().GetHeight()));
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* CurrentContext = glfwGetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        glfwMakeContextCurrent(CurrentContext);
    }
}

void COpenGLImGuiLayer::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    IO.ConfigWindowsMoveFromTitleBarOnly = true;
    
    IO.FontDefault = IO.Fonts->AddFontFromFileTTF("Resources/Fonts/Inter_28pt-Regular.ttf", 17.0f);

    ImGui::StyleColorsDark();

    ImGuiStyle& Style = ImGui::GetStyle();
    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        Style.WindowRounding = 0.0f;
        Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    SetDarkThemeColors();
    
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(CApplication::GetInstance().GetWindow().GetHandle()), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void COpenGLImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void COpenGLImGuiLayer::OnImGuiRender()
{
    bool bShowDemoWindow = true;
    ImGui::ShowDemoWindow(&bShowDemoWindow);
}
