#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Camera/OrthographicCameraController.h"

#include <imgui.h>

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer()
        : CLayer("Moonlight Editor Layer"), m_CameraController(1600.0f / 900.0f)
    {
    }

    void OnAttach() override
    {
        m_Renderer2D = std::make_shared<CRenderer2D>();
        m_PlaceholderTexture = ITexture::Create("Resources/Textures/Placeholder.png");
        m_MushroomTexture = ITexture::Create("Resources/Textures/Mushroom.png");
    }
    
    void OnUpdate(float DeltaTime) override
    {
        m_CameraController.OnUpdate(DeltaTime);
        
        m_QuadRotation += 20.0f * DeltaTime;
    }

    void OnRender() override
    {
        m_Renderer2D->ResetStats();
        
        CRenderer::BeginScene(m_CameraController.GetCamera());
        m_Renderer2D->BeginFrame();
        
        m_Renderer2D->DrawQuad({ 0.0f, 0.0f }, { 15.0f, 15.0f }, m_PlaceholderTexture);
        
        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        {
            for (float y = -5.0f; y < 5.0f; y += 0.5f)
                m_Renderer2D->DrawQuad({ x, y },  { 0.45f, 0.45f }, { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f });
        }
        
        m_Renderer2D->DrawRotatedQuad({ -1.0f, 0.5f }, { 0.5f, 0.5f }, m_QuadRotation, { 0.9f, 0.3f, 0.2f, 1.0f });
        m_Renderer2D->DrawRotatedQuad({ 0.0f, 0.5f }, { 0.7f, 0.5f }, m_QuadRotation, { 0.3f, 0.9f, 0.2f, 1.0f });
        m_Renderer2D->DrawRotatedQuad({ 1.0f, 0.5f }, { 0.7f, 0.5f },  m_QuadRotation, { 0.3f, 0.2f, 0.9f, 1.0f });
        m_Renderer2D->DrawRotatedQuad({ -0.7f, -0.5f }, { 1.0f, 1.0f }, -m_QuadRotation, m_MushroomTexture, 1.0f);
        m_Renderer2D->DrawRotatedQuad({ 0.7f, -0.5f }, { 1.0f, 1.0f }, -m_QuadRotation, m_MushroomTexture, 1.0f);
        
        m_Renderer2D->EndFrame();
        CRenderer::EndScene();
    }
    
    void OnImGuiRender() override
    {
        ImGui::Begin("Renderer2D Statistics");
      
        ImGui::Text("Draw Calls: %u", m_Renderer2D->GetStats().DrawCalls);
        ImGui::Text("Quads: %u", m_Renderer2D->GetStats().QuadCount);
        ImGui::Text("Vertex Count: %u", m_Renderer2D->GetStats().GetTotalVertexCount());
        ImGui::Text("Index Count: %u", m_Renderer2D->GetStats().GetTotalIndexCount());
        
        ImGui::End();
    }
    
    void OnEvent(IEvent& Event) override
    {
        m_CameraController.OnEvent(Event);
    }
private:
    std::shared_ptr<CRenderer2D> m_Renderer2D;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
    std::shared_ptr<ITexture> m_MushroomTexture;

    COrthographicCameraController m_CameraController;

    float m_QuadRotation = 0.0f;
};

class CMoonlightEditorApplication : public CApplication
{
public:
    CMoonlightEditorApplication(const CApplicationSpecification& Specification)
        : CApplication(Specification)
    {
        PushLayer(new CMoonlightEditorLayer());
    }
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount)
{
    CApplicationSpecification MoonlightEditorApplicationSpecification;
    MoonlightEditorApplicationSpecification.Name = "Moonlight Editor";
    MoonlightEditorApplicationSpecification.CommandLineArguments = CCommandLineArguments(Arguments, ArgumentCount);

    return new CMoonlightEditorApplication(MoonlightEditorApplicationSpecification);
}
