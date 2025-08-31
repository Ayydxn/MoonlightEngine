#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer()
        : CLayer("Moonlight Editor Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
    }

    void OnAttach() override
    {
        m_Renderer2D = std::make_shared<CRenderer2D>();
        m_PlaceholderTexture = ITexture::Create("Resources/Textures/Placeholder.png");
        m_MushroomTexture = ITexture::Create("Resources/Textures/Mushroom.png");
    }

    void OnRender() override
    {
        CRenderer::BeginScene(m_Camera);
        m_Renderer2D->BeginFrame();

        //m_Renderer2D->DrawRotatedQuad({ 0.0f, 0.0f }, { 1.5f, 1.5f }, 45.0f, { 0.9f, 0.2f, 0.3f, 1.0f });
        //m_Renderer2D->DrawRotatedQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, 45.0f, m_PlaceholderTexture, 10.0f);

        m_Renderer2D->DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, { 0.9f, 0.3f, 0.2f, 1.0f });
        m_Renderer2D->DrawQuad({ 0.0f, 0.0f }, { 0.7f, 0.5f }, { 0.3f, 0.9f, 0.2f, 1.0f });
        m_Renderer2D->DrawQuad({ 1.0f, 0.0f }, { 0.7f, 0.5f }, { 0.3f, 0.2f, 0.9f, 1.0f });
        //m_Renderer2D->DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_MushroomTexture);
        //m_Renderer2D->DrawQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, m_PlaceholderTexture);

        m_Renderer2D->EndFrame();
        CRenderer::EndScene();
    }

private:
    std::shared_ptr<CRenderer2D> m_Renderer2D;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
    std::shared_ptr<ITexture> m_MushroomTexture;

    COrthographicCamera m_Camera;
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
