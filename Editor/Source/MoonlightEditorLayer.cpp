#include "MoonlightEditorLayer.h"
#include "Application/Application.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/SpriteRendererComponent.h"
#include "Scene/Components/TransformComponent.h"
#include "Scene/Entity/Entity.h"

#include <imgui.h>

CMoonlightEditorLayer::CMoonlightEditorLayer()
    : CLayer("Moonlight Editor")
{
}

void CMoonlightEditorLayer::OnAttach()
{
    m_SceneRenderer = std::make_shared<CSceneRenderer>();
    m_ActiveScene = std::make_shared<CScene>();
    
    CEntity Entity = m_ActiveScene->CreateEntity("Red Square");
    Entity.AddComponent<CSpriteRendererComponent>(glm::vec4 { 1.0f, 0.0f, 0.0f, 1.0f });
    
    CEntity SecondEntity = m_ActiveScene->CreateEntity("Green Square");
    SecondEntity.GetComponent<CTransformComponent>().Position = { 1.0f, 0.5f, 0.0f };
    SecondEntity.AddComponent<CSpriteRendererComponent>(glm::vec4 { 0.0f, 1.0f, 0.0f, 1.0f });
    
    CEntity CameraEntity = m_ActiveScene->CreateEntity("Camera");
    CameraEntity.AddComponent<CCameraComponent>();
    
    m_SceneViewportPanel = std::make_unique<CSceneViewportPanel>(m_SceneRenderer, m_ActiveScene);
    m_SceneHierarchyPanel = std::make_unique<CSceneHierarchyPanel>(m_ActiveScene);
}

void CMoonlightEditorLayer::OnUpdate(float DeltaTime)
{
    m_ActiveScene->OnUpdate(DeltaTime);
    m_SceneViewportPanel->OnUpdate(DeltaTime);
}

void CMoonlightEditorLayer::OnEvent(IEvent& Event)
{
    m_SceneViewportPanel->OnEvent(Event);
}

void CMoonlightEditorLayer::OnRender()
{
    m_SceneViewportPanel->OnRender();
}

void CMoonlightEditorLayer::OnImGuiRender()
{
    UI_RenderDockSpace();
    
    m_SceneViewportPanel->OnImGuiRender();
    m_SceneHierarchyPanel->OnImGuiRender();
}

void CMoonlightEditorLayer::UI_RenderDockSpace()
{
    const ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(Viewport->WorkPos);
    ImGui::SetNextWindowSize(Viewport->WorkSize);
    ImGui::SetNextWindowViewport(Viewport->ID);

    constexpr ImGuiWindowFlags DockSpaceWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpaceWindow", nullptr, DockSpaceWindowFlags);

    ImGui::PopStyleVar(3);

    UI_RenderMenuBar();

    ImGui::DockSpace(ImGui::GetID("Moonlight Editor DockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
}

void CMoonlightEditorLayer::UI_RenderMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                CApplication::GetInstance().Close();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}
