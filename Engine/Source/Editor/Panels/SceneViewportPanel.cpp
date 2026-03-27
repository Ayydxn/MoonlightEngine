#include "MoonlightPCH.h"
#include "SceneViewportPanel.h"
#include "Application/Application.h"

#include <imgui.h>

CSceneViewportPanel::CSceneViewportPanel(const std::shared_ptr<CSceneRenderer>& SceneRenderer, const std::shared_ptr<CScene>& Scene)
    : m_SceneRenderer(SceneRenderer), m_ActiveScene(Scene)
{
    m_Framebuffer = IFramebuffer::Create({
        .Width = 1600,
        .Height = 900,
        .Samples = 1,
        .Attachments = {
            { ETextureFormat::RGBA8 }
        }
    });
}

void CSceneViewportPanel::OnUpdate(float DeltaTime)
{
    if (bIsFocused)
        m_Camera.OnUpdate(DeltaTime);
}

void CSceneViewportPanel::OnEvent(IEvent& Event)
{
    m_Camera.OnEvent(Event);
}

void CSceneViewportPanel::OnRender() const
{
    if (!m_ActiveScene || !m_SceneRenderer)
        return;
    
    m_Framebuffer->Bind();
        
    m_ActiveScene->OnRenderEditor(m_SceneRenderer, m_Camera);
        
    m_Framebuffer->Unbind();
}

void CSceneViewportPanel::OnImGuiRender()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene Viewport");
    
    bIsFocused = ImGui::IsWindowFocused();
    
    CApplication::GetInstance().GetImGuiLayer()->BlockEvents(!bIsFocused || !ImGui::IsWindowHovered());
    
    const ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
    
    if (ViewportPanelSize.x > 0.0f && ViewportPanelSize.y > 0.0f && (m_Size.x != ViewportPanelSize.x || m_Size.y != ViewportPanelSize.y))
    {
        m_Size = { ViewportPanelSize.x, ViewportPanelSize.y };
        
        m_Framebuffer->Resize(static_cast<uint32>(m_Size.x), static_cast<uint32>(m_Size.y));
        m_Camera.SetViewportSize(m_Size.x, m_Size.y);
        
        if (m_ActiveScene)
            m_ActiveScene->OnViewportResize(static_cast<uint32>(m_Size.x), static_cast<uint32>(m_Size.y));
    }

    ImGui::Image(m_Framebuffer->GetColorAttachment(0)->GetNativeHandle(), { m_Size.x, m_Size.y }, ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();
}
