#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.h"
#include "Input/Input.h"
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
    
    void OnUpdate(float DeltaTime) override
    {
        HandleCameraMovement(DeltaTime);
        HandleCameraRotation(DeltaTime);
        
        m_QuadRotation += 20.0f * DeltaTime;
    }

    void OnRender() override
    {
        CRenderer::BeginScene(m_Camera);
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
private:
    void HandleCameraMovement(float DeltaTime)
    {
        if (CInput::GetKeyHeld(CKeys::W))
        {
            glm::vec3 CameraPosition = m_Camera.GetPosition();
            CameraPosition.x += -glm::sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            CameraPosition.y += glm::cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            
            m_Camera.SetPosition(CameraPosition);
        }
        
        if (CInput::GetKeyHeld(CKeys::A))
        {
            glm::vec3 CameraPosition = m_Camera.GetPosition();
            CameraPosition.x -= glm::cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            CameraPosition.y -= glm::sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            
            m_Camera.SetPosition(CameraPosition);
        }
        
        if (CInput::GetKeyHeld(CKeys::S))
        {
            glm::vec3 CameraPosition = m_Camera.GetPosition();
            CameraPosition.x -= -glm::sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            CameraPosition.y -= glm::cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            
            m_Camera.SetPosition(CameraPosition);
        }
        
        if (CInput::GetKeyHeld(CKeys::D))
        {
            glm::vec3 CameraPosition = m_Camera.GetPosition();
            CameraPosition.x += glm::cos(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            CameraPosition.y += glm::sin(glm::radians(m_CameraRotation)) * m_CameraMovementSpeed * DeltaTime;
            
            m_Camera.SetPosition(CameraPosition);
        }
    }
    
    void HandleCameraRotation(float DeltaTime)
    {
        if (CInput::GetKeyHeld(CKeys::E))
            m_CameraRotation += m_CameraRotationSpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::Q))
            m_CameraRotation -= m_CameraRotationSpeed * DeltaTime;
        
        if (m_CameraRotation > 180.0f)
        {
            m_CameraRotation -= 360.0f;
        }
        else if (m_CameraRotation <= -180.0f)
        {
            m_CameraRotation += 360.0f;
        }
        
        m_Camera.SetRotation(m_CameraRotation);
    }
private:
    std::shared_ptr<CRenderer2D> m_Renderer2D;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
    std::shared_ptr<ITexture> m_MushroomTexture;

    COrthographicCamera m_Camera;
    float m_CameraRotation = 0.0f;
    float m_CameraMovementSpeed = 2.5f;
    float m_CameraRotationSpeed = 50.0f;
    
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
