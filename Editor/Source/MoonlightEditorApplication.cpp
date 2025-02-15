#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "RHICore/Shader.h"
#include "RHICore/Texture.h"

#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>

class CMoonlightEditorLayer : public CLayer
{
public:
    CMoonlightEditorLayer()
        : CLayer("Moonlight Editor Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {}

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
        m_MushroomTexture = ITexture::Create("Resources/Textures/Mushroom.png");
    }

    // Transforms Test
    void OnFixedUpdate(float DeltaTime) override
    {
        /*----------------*/
        /* -- Movement -- */
        /*----------------*/
        
        if (CInput::GetKeyHeld(CKeys::D))
            m_SquarePosition.x += m_SquareMovementSpeed * DeltaTime;

        if (CInput::GetKeyHeld(CKeys::A))
            m_SquarePosition.x -= m_SquareMovementSpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::W))
            m_SquarePosition.y += m_SquareMovementSpeed * DeltaTime;

        if (CInput::GetKeyHeld(CKeys::S))
            m_SquarePosition.y -= m_SquareMovementSpeed * DeltaTime;

        /*----------------*/
        /* -- Rotation -- */
        /*----------------*/

        if (CInput::GetKeyHeld(CKeys::E))
            m_SquareRotation += m_SquareRotationSpeed * DeltaTime;

        if (CInput::GetKeyHeld(CKeys::Q))
            m_SquareRotation -= m_SquareRotationSpeed * DeltaTime;

        /*---------------*/
        /* -- Scaling -- */
        /*---------------*/

        if (CInput::GetKeyHeld(CKeys::Z))
        {
            m_SquareScale.x += m_SquareScaleSpeed * DeltaTime;
            m_SquareScale.y += m_SquareScaleSpeed * DeltaTime;
            m_SquareScale.z += m_SquareScaleSpeed * DeltaTime;
        }

        if (CInput::GetKeyHeld(CKeys::X) && m_SquareScale.x > 0.0f)
        {
            m_SquareScale.x -= m_SquareScaleSpeed * DeltaTime;
            m_SquareScale.y -= m_SquareScaleSpeed * DeltaTime;
            m_SquareScale.z -= m_SquareScaleSpeed * DeltaTime;
        }
    }

    void OnRender() override
    {
        CRenderer::BeginScene(m_Camera);

        const auto Transform = glm::translate(glm::mat4(1.0f), m_SquarePosition) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_SquareRotation), glm::vec3(0, 0, 1)) *
                glm::scale(glm::mat4(1.0f), m_SquareScale);
        
        CRenderer::DrawIndexed({ m_Shader, m_VertexBuffer, m_IndexBuffer, m_GraphicsPipeline, m_PlaceholderTexture }, Transform);
        CRenderer::DrawIndexed({ m_Shader, m_VertexBuffer, m_IndexBuffer, m_GraphicsPipeline, m_MushroomTexture });

        CRenderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Square Transform");

        ImGui::Text("Position: %f, %f, %f", m_SquarePosition.x, m_SquarePosition.y, m_SquarePosition.z);
        ImGui::Text("Rotation: %f", m_SquareRotation);
        ImGui::Text("Square Scale: %f, %f, %f", m_SquareScale.x, m_SquareScale.y, m_SquareScale.z);

        ImGui::NewLine();

        ImGui::Text("Press WASD to Move");
        ImGui::Text("Press Q and E to Rotate");
        ImGui::Text("Press Z and X to Scale");

        ImGui::End();
    }
private:
    std::shared_ptr<IVertexBuffer> m_VertexBuffer;
    std::shared_ptr<IIndexBuffer> m_IndexBuffer;
    std::shared_ptr<IGraphicsPipeline> m_GraphicsPipeline;
    std::shared_ptr<IShader> m_Shader;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
    std::shared_ptr<ITexture> m_MushroomTexture;

    glm::vec3 m_SquarePosition = glm::vec3(0.0f);
    float m_SquareRotation = 0.0f;
    glm::vec3 m_SquareScale = glm::vec3(1.0f);

    COrthographicCamera m_Camera;

    float m_SquareMovementSpeed = 0.025f;
    float m_SquareRotationSpeed = 0.5f;
    float m_SquareScaleSpeed = 0.025f;
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