#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Renderer/Renderer.h"
#include "RHICore/Shader.h"
#include "RHICore/Texture.h"

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer()
        : CLayer("Moonlight Editor Layer") {}

    void OnAttach() override
    {
        constexpr float Vertices[20] =
        { 
            // Positions            // Texture Coords 
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f
        };

        constexpr uint32 Indices[] = { 0, 1, 2, 2, 3, 0 };

        const CVertexBufferLayout VertexBufferLayout =
        {
            { "Positions", EShaderDataType::Float3 },
            { "TextureCoords", EShaderDataType::Float2 }
        };

        CGraphicsPipelineSpecification GraphicsPipelineSpecification;
        GraphicsPipelineSpecification.VertexBufferLayout = VertexBufferLayout;

        m_Shader = CRenderer::GetShaderLibrary()->GetShader("DefaultShader");

        m_VertexBuffer = IVertexBuffer::Create(Vertices, sizeof(Vertices));
        m_IndexBuffer = IIndexBuffer::Create(Indices, sizeof(Indices));
        m_GraphicsPipeline = IGraphicsPipeline::Create(GraphicsPipelineSpecification);

        m_PlaceholderTexture = ITexture::Create("Resources/Textures/Placeholder.png");
    }

    void OnRender() override
    {
        CRenderer::DrawIndexed({ m_Shader, m_VertexBuffer, m_IndexBuffer, m_GraphicsPipeline, m_PlaceholderTexture });
    }
private:
    std::shared_ptr<IVertexBuffer> m_VertexBuffer;
    std::shared_ptr<IIndexBuffer> m_IndexBuffer;
    std::shared_ptr<IGraphicsPipeline> m_GraphicsPipeline;
    std::shared_ptr<IShader> m_Shader;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
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