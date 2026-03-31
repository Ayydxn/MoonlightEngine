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
    IImGuiLayer::OnAttach();
    
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(CApplication::GetInstance().GetWindow().GetHandle()), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void COpenGLImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
