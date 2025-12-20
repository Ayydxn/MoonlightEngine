#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Camera/OrthographicCameraController.h"

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
        CRenderer::BeginScene(m_CameraController.GetCamera());
        m_Renderer2D->BeginFrame();
        
        m_Renderer2D->DrawQuad({ 0.0f, 0.0f }, { 10.0f, 10.0f }, m_PlaceholderTexture);
        
        m_Renderer2D->DrawRotatedQuad({ -1.0f, 0.5f }, { 0.5f, 0.5f }, m_QuadRotation, { 0.9f, 0.3f, 0.2f, 1.0f });
        m_Renderer2D->DrawRotatedQuad({ 0.0f, 0.5f }, { 0.7f, 0.5f }, m_QuadRotation, { 0.3f, 0.9f, 0.2f, 1.0f });
        m_Renderer2D->DrawRotatedQuad({ 1.0f, 0.5f }, { 0.7f, 0.5f },  m_QuadRotation, { 0.3f, 0.2f, 0.9f, 1.0f });
        m_Renderer2D->DrawRotatedQuad({ -0.7f, -0.5f }, { 1.0f, 1.0f }, -m_QuadRotation, m_MushroomTexture, 1.0f);
        m_Renderer2D->DrawRotatedQuad({ 0.7f, -0.5f }, { 1.0f, 1.0f }, -m_QuadRotation, m_MushroomTexture, 1.0f);
        
        m_Renderer2D->EndFrame();
        CRenderer::EndScene();
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
