#pragma once

#include "Events/MouseEvents.h"
#include "Renderer/Camera/Camera.h"

class MOONLIGHT_API CEditorViewportCamera : public CCamera
{
public:
    CEditorViewportCamera() = default;
    CEditorViewportCamera(float FOV, float AspectRatio, float NearClip, float FarClip);
    
    void OnUpdate(float DeltaTime);
    void OnEvent(IEvent& Event);
    
    float GetDistance() const { return m_Distance; }
    void SetDistance(float Distance) { m_Distance = Distance; }

    void SetViewportSize(float Width, float Height)
    {
        m_ViewportWidth = Width;
        m_ViewportHeight = Height;
        
        UpdateProjectionMatrix();
    }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;
    const glm::vec3& GetPosition() const { return m_Position; }
    glm::quat GetOrientation() const;

    float GetPitch() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }
private:
    void UpdateProjectionMatrix();
    void UpdateViewMatrix();

    void MousePan(const glm::vec2& Delta);
    void MouseRotate(const glm::vec2& Delta);
    void MouseZoom(float Delta);

    glm::vec3 CalculatePosition() const;
    std::pair<float, float> CalculatePanSpeed() const;
    float CalculateRotationSpeed() const { return 0.8f; }
    float CalculateZoomSpeed() const;
private:
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_FocalPoint = glm::vec4(0.0f);
    glm::vec2 m_InitialMousePosition = glm::vec2(0.0f);
    glm::vec2 m_LastMousePosition = glm::vec2(0.0f);
    
    float m_FOV = 90.0f;
    float m_AspectRatio = 1.778f;
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;
    float m_Distance = 10.0f;
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
    float m_ViewportWidth = 1280.0f;
    float m_ViewportHeight = 720.0f;
};
