#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Input/Input.h"
#include "RHICore/IndexBuffer.h"
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

        m_VertexBuffer = FVertexBuffer::Create(Vertices, sizeof(Vertices));

        m_IndexBuffer = FIndexBuffer::Create(Indices, sizeof(Indices));
    }
private:
    std::shared_ptr<FVertexBuffer> m_VertexBuffer;
    std::shared_ptr<FIndexBuffer> m_IndexBuffer;
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