#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexBufferLayout.h"
#include "RHICore/GraphicsPipeline.h"
#include "RHICore/IndexBuffer.h"
#include "RHICore/Shader.h"
#include "RHICore/VertexBuffer.h"

class FMoonlightEditorLayer : public FLayer
{
public:
    FMoonlightEditorLayer()
        : FLayer("Moonlight Editor Layer") {}

    void OnAttach() override
    {
        constexpr float Vertices[9] =
        {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        constexpr int32 Indices[3] =
        {
            0, 1, 2
        };

        const FVertexBufferLayout VertexBufferLayout =
        {
            { EShaderDataType::Float3, "Positions" }
        };

        FGraphicsPipelineSpecification GraphicsPipelineSpecification;
        GraphicsPipelineSpecification.VertexBufferLayout = VertexBufferLayout;

        m_Shader = FShader::Create("Resources/Shaders/DefaultShader.glsl");

        m_VertexBuffer = FVertexBuffer::Create(Vertices, sizeof(Vertices));

        m_IndexBuffer = FIndexBuffer::Create(Indices, sizeof(Indices));

        m_GraphicsPipeline = FGraphicsPipeline::Create(GraphicsPipelineSpecification);

        m_VertexBuffer->Bind();
        m_IndexBuffer->Bind();
    }

    void OnRender() override
    {
        FRHICommandPacket RHICommandPacket;
        RHICommandPacket.IndexBuffer = m_IndexBuffer;

        FRenderer::ClearColor(0.45f, 0.55f, 0.65f, 1.0f);

        m_Shader->Bind();
        m_GraphicsPipeline->Bind();
        FRenderer::DrawIndexedPrimitive(RHICommandPacket);
    }
private:
    std::shared_ptr<FShader> m_Shader;
    std::shared_ptr<FVertexBuffer> m_VertexBuffer;
    std::shared_ptr<FIndexBuffer> m_IndexBuffer;
    std::shared_ptr<FGraphicsPipeline> m_GraphicsPipeline;
};

class FMoonlightEditorApplication : public FApplication
{
public:
    FMoonlightEditorApplication(const FApplicationSpecification& Specification)
        : FApplication(Specification)
    {
        PushLayer(new FMoonlightEditorLayer());
    }
};

FApplication* CreateApplication(char** Arguments, int32 ArgumentCount)
{
    FApplicationSpecification MoonlightEditorApplicationSpecification;
    MoonlightEditorApplicationSpecification.Name = "Moonlight Editor";
    MoonlightEditorApplicationSpecification.CommandLineArguments = FCommandLineArguments(Arguments, ArgumentCount);

    return new FMoonlightEditorApplication(MoonlightEditorApplicationSpecification);
}