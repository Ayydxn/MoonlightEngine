#include "MoonlightPCH.h"
#include "OrthographicCameraController.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "Input/Input.h"

COrthographicCameraController::COrthographicCameraController(float AspectRatio, bool bRotatable)
    : m_Camera(-AspectRatio * m_ZoomLevel, AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
      m_AspectRatio(AspectRatio), bIsRotatable(bRotatable)
{
    
}

void COrthographicCameraController::OnUpdate(float RenderDeltaTime)
{
    if (CInput::GetKeyHeld(CKeys::W))
    {
        glm::vec3 CameraPosition = m_Camera.GetPosition();
        CameraPosition.x += -glm::sin(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;
        CameraPosition.y += glm::cos(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;

        m_Camera.SetPosition(CameraPosition);
    }

    if (CInput::GetKeyHeld(CKeys::A))
    {
        glm::vec3 CameraPosition = m_Camera.GetPosition();
        CameraPosition.x -= glm::cos(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;
        CameraPosition.y -= glm::sin(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;

        m_Camera.SetPosition(CameraPosition);
    }

    if (CInput::GetKeyHeld(CKeys::S))
    {
        glm::vec3 CameraPosition = m_Camera.GetPosition();
        CameraPosition.x -= -glm::sin(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;
        CameraPosition.y -= glm::cos(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;

        m_Camera.SetPosition(CameraPosition);
    }

    if (CInput::GetKeyHeld(CKeys::D))
    {
        glm::vec3 CameraPosition = m_Camera.GetPosition();
        CameraPosition.x += glm::cos(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;
        CameraPosition.y += glm::sin(glm::radians(m_CurrentRotation)) * m_MovementSpeed * RenderDeltaTime;

        m_Camera.SetPosition(CameraPosition);
    }
}

void COrthographicCameraController::OnEvent(IEvent& Event)
{
    CEventDispatcher EventDispatcher(Event);
    EventDispatcher.Dispatch<CMouseScrolledEvent>([this](const CMouseScrolledEvent& MouseScrolledEvent)
    {
        m_ZoomLevel -= MouseScrolledEvent.GetYOffset() * 0.25f;
        m_ZoomLevel = glm::max(m_ZoomLevel, 0.25f);

        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return true;
    });

    EventDispatcher.Dispatch<CWindowResizeEvent>([this](const CWindowResizeEvent& WindowResizeEvent)
    {
        m_AspectRatio = static_cast<float>(WindowResizeEvent.GetWidth()) / static_cast<float>(WindowResizeEvent.
            GetHeight());

        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return true;
    });
}
