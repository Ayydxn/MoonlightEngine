#include "MoonlightEditorLayer.h"
#include "Application/Application.h"

CMoonlightEditorLayer::CMoonlightEditorLayer()
    : CLayer("Moonlight Editor")
{
}

void CMoonlightEditorLayer::OnAttach()
{
    m_Renderer2D = std::make_shared<CRenderer2D>();
    m_PlaceholderTexture = ITexture::Create("Resources/Textures/Placeholder.png");
    m_MushroomTexture = ITexture::Create("Resources/Textures/Mushroom.png");
    m_SceneFramebuffer = IFramebuffer::Create({
        .Width = 1600,
        .Height = 900,
        .Samples = 1,
        .Attachments = {
            { ETextureFormat::RGBA8 }
        }
    });
}

void CMoonlightEditorLayer::OnUpdate(float DeltaTime)
{
    m_ViewportCamera.OnUpdate(DeltaTime);
    
    m_QuadRotation += 20.0f * DeltaTime;
}

void CMoonlightEditorLayer::OnEvent(IEvent& Event)
{
    m_ViewportCamera.OnEvent(Event);
}

void CMoonlightEditorLayer::OnRender()
{
    m_Renderer2D->ResetStats();

    m_SceneFramebuffer->Bind();
    
    m_Renderer2D->BeginFrame(m_ViewportCamera);

    m_Renderer2D->DrawQuad({0.0f, 0.0f}, {15.0f, 15.0f}, m_PlaceholderTexture);

    for (float x = -5.0f; x < 5.0f; x += 0.5f)
    {
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
            m_Renderer2D->DrawQuad({x, y}, {0.45f, 0.45f}, {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f});
    }

    m_Renderer2D->DrawRotatedQuad({-1.0f, 0.5f}, {0.5f, 0.5f}, m_QuadRotation, {0.9f, 0.3f, 0.2f, 1.0f});
    m_Renderer2D->DrawRotatedQuad({0.0f, 0.5f}, {0.7f, 0.5f}, m_QuadRotation, {0.3f, 0.9f, 0.2f, 1.0f});
    m_Renderer2D->DrawRotatedQuad({1.0f, 0.5f}, {0.7f, 0.5f}, m_QuadRotation, {0.3f, 0.2f, 0.9f, 1.0f});
    m_Renderer2D->DrawRotatedQuad({-0.7f, -0.5f}, {1.0f, 1.0f}, -m_QuadRotation, m_MushroomTexture, 1.0f);
    m_Renderer2D->DrawRotatedQuad({0.7f, -0.5f}, {1.0f, 1.0f}, -m_QuadRotation, m_MushroomTexture, 1.0f);

    m_Renderer2D->EndFrame();

    m_SceneFramebuffer->Unbind();
}

void CMoonlightEditorLayer::OnImGuiRender()
{
    UI_RenderDockSpace();
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

    UI_RenderViewport();
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

void CMoonlightEditorLayer::UI_RenderViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene Viewport");
    
    const ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

    if (ViewportPanelSize.x > 0.0f && ViewportPanelSize.y > 0.0f && (m_ViewportSize.x != ViewportPanelSize.x || m_ViewportSize.y != ViewportPanelSize.y))
    {
        m_ViewportSize = ViewportPanelSize;
        
        m_SceneFramebuffer->Resize(static_cast<uint32>(m_ViewportSize.x), static_cast<uint32>(m_ViewportSize.y));
        m_ViewportCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    }

    ImGui::Image(m_SceneFramebuffer->GetColorAttachment(0)->GetNativeHandle(), m_ViewportSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();
}
