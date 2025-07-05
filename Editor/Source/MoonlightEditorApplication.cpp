#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer()
        : CLayer("Moonlight Editor Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {}

    void OnAttach() override
    {
        m_PlaceholderTexture = ITexture::Create("Resources/Textures/Placeholder.png");
        m_MushroomTexture = ITexture::Create("Resources/Textures/Mushroom.png");
    }
    
    void OnRender() override
    {
        CRenderer::BeginScene(m_Camera);

        //CRenderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.5f, 1.5f }, 45.0f, { 0.9f, 0.2f, 0.3f, 1.0f });
        CRenderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 2.0f, 2.0f }, 45.0f, m_PlaceholderTexture, 10.0f);
        
        CRenderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, { 0.3f, 0.2f, 0.9f, 1.0f });
        CRenderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_MushroomTexture);
        CRenderer2D::DrawQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, m_PlaceholderTexture);
       
        CRenderer::EndScene();
    }
private:
    std::shared_ptr<IVertexBuffer> m_VertexBuffer;
    std::shared_ptr<IIndexBuffer> m_IndexBuffer;
    std::shared_ptr<IGraphicsPipeline> m_GraphicsPipeline;
    std::shared_ptr<IShader> m_Shader;
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