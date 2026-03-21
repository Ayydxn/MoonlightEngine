#include "MoonlightPCH.h"
#include "EditorViewportCamera.h"
#include "Input/Input.h"

#include <imgui.h>
#include <glm/ext/matrix_clip_space.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

CEditorViewportCamera::CEditorViewportCamera(float FOV, float AspectRatio, float NearClip, float FarClip)
    : CCamera(glm::perspective(glm::radians(FOV), AspectRatio, NearClip, FarClip)), m_FOV(FOV), m_AspectRatio(AspectRatio), m_NearClip(NearClip),
      m_FarClip(FarClip)
{
    UpdateViewMatrix();
}

void CEditorViewportCamera::OnUpdate(float DeltaTime)
{
    const auto [MouseX, MouseY] = CInput::GetMousePosition();
    const glm::vec2 MousePosition = glm::vec2(MouseX, MouseY);
    const glm::vec2 MouseDelta = (MousePosition - m_LastMousePosition) * 0.003f;
    const bool bMiddleMouseButtonHeld = CInput::GetMouseButtonHeld(CKeys::MiddleMouseButton);
    const bool bLeftMouseButtonHeld = CInput::GetMouseButtonHeld(CKeys::LeftMouseButton);
    const bool bRightMouseButtonHeld = CInput::GetMouseButtonHeld(CKeys::RightMouseButton);
    
    // Lock the cursor if we're doing any camera movement
    if (bMiddleMouseButtonHeld || bLeftMouseButtonHeld || bRightMouseButtonHeld)
    {
        CInput::SetCursorMode(ECursorMode::Locked);
        
        // (Ayydxn) A bit of a hack to prevent the cursor from interacting with ImGui elements while we're controlling the camera.
        // Should probably move this somewhere else.
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; 
    }
    else
    {
        CInput::SetCursorMode(ECursorMode::Normal);
        
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    
    // Camera controls
    if (bMiddleMouseButtonHeld)
    {
        MousePan(MouseDelta);
    }
    else if (bLeftMouseButtonHeld)
    {
        MouseRotate(MouseDelta);
    }
    else if (bRightMouseButtonHeld)
    {
        // TODO: (Ayydxn) Make this configurable by the user.
        constexpr float FlySpeed = 5.0f;
        
        MouseRotate(MouseDelta);
        
        if (CInput::GetKeyHeld(CKeys::W))
            m_FocalPoint += GetForwardDirection() * FlySpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::S))
            m_FocalPoint -= GetForwardDirection() * FlySpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::A))
            m_FocalPoint -= GetRightDirection() * FlySpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::D))
            m_FocalPoint += GetRightDirection() * FlySpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::E))
            m_FocalPoint += GetUpDirection() * FlySpeed * DeltaTime;
        
        if (CInput::GetKeyHeld(CKeys::Q))
            m_FocalPoint -= GetUpDirection() * FlySpeed * DeltaTime;
    }
    
    m_LastMousePosition = MousePosition;
    
    UpdateViewMatrix();
}

void CEditorViewportCamera::OnEvent(IEvent& Event)
{
    CEventDispatcher EventDispatcher(Event);
    EventDispatcher.Dispatch<CMouseScrolledEvent>([this](const CMouseScrolledEvent& MouseScrolledEvent)
    {
        const float ScrollDelta = MouseScrolledEvent.GetYOffset() * 0.1f;
        
        MouseZoom(ScrollDelta);
        UpdateViewMatrix();
        
        return false;
    });
}

void CEditorViewportCamera::UpdateProjectionMatrix()
{
    m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void CEditorViewportCamera::UpdateViewMatrix()
{
    m_Position = CalculatePosition();
    
    const glm::quat Orientation = GetOrientation();
    
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(Orientation);
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

void CEditorViewportCamera::MousePan(const glm::vec2& Delta)
{
    const auto [XSpeed, YSpeed] = CalculatePanSpeed();
    
    m_FocalPoint += -GetRightDirection() * Delta.x * XSpeed * m_Distance;
    m_FocalPoint += GetUpDirection() * Delta.y * YSpeed * m_Distance;
}

void CEditorViewportCamera::MouseRotate(const glm::vec2& Delta)
{
    const float YawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    
    m_Pitch += Delta.y * CalculateRotationSpeed();
    m_Yaw += YawSign * Delta.x * CalculateRotationSpeed();
}

void CEditorViewportCamera::MouseZoom(float Delta)
{
    m_Distance -= Delta * CalculateZoomSpeed();
    
    if (m_Distance < 1.0f)
    {
        m_FocalPoint += GetForwardDirection();
        m_Distance = 1.0f;
    }
}

glm::vec3 CEditorViewportCamera::CalculatePosition() const
{
    return m_FocalPoint - GetForwardDirection() * m_Distance;
}

std::pair<float, float> CEditorViewportCamera::CalculatePanSpeed() const
{
    // Pan speed scales with viewport size but is capped to prevent excessive speeds on very large viewports.
    // The quadratic formula was curve-fitted to produce a smooth, natural-feeling pan speed across typical viewport sizes.
    constexpr float MaxPanSpeed = 2.4f;

    // Normalize viewport dimensions to a [0, MaxPanSpeed] range
    const float NormalizedX = glm::min(m_ViewportWidth  / 1000.0f, MaxPanSpeed);
    const float NormalizedY = glm::min(m_ViewportHeight / 1000.0f, MaxPanSpeed);

    // Quadratic: f(t) = 0.0366t^2 - 0.1778t + 0.3021
    // Produces a gentle ease-in so small viewports pan slowly and large ones pan faster
    const auto QuadraticScale = [](const float T) -> float
    {
        return 0.0366f * (T * T) - 0.1778f * T + 0.3021f;
    };

    const float XFactor = QuadraticScale(NormalizedX);
    const float YFactor = QuadraticScale(NormalizedY);

    return { XFactor, YFactor };
}

float CEditorViewportCamera::CalculateZoomSpeed() const
{
    constexpr float MaxSpeed = 100.0f;
    const float Distance = glm::max(m_Distance * 0.2f, 0.0f);
    const float Speed = glm::min(Distance * Distance, MaxSpeed);
    
    return Speed;
}

glm::vec3 CEditorViewportCamera::GetUpDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 CEditorViewportCamera::GetRightDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 CEditorViewportCamera::GetForwardDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat CEditorViewportCamera::GetOrientation() const
{
    return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}
