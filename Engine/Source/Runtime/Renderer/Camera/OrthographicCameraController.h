#pragma once

#include "OrthographicCamera.h"
#include "Core/CoreDefines.h"
#include "Events/Event.h"

class MOONLIGHT_API COrthographicCameraController
{
public:
    COrthographicCameraController(float AspectRatio, bool bRotatable = true);
    void OnUpdate(float RenderDeltaTime);
    
    void OnEvent(IEvent& Event);
    
    const COrthographicCamera& GetCamera() const { return m_Camera; }
private:
    float m_ZoomLevel = 1.0f;
    COrthographicCamera m_Camera;
    
    bool bIsRotatable;
    float m_AspectRatio;
    
    float m_CurrentRotation = 0.0f;
    float m_MovementSpeed = 2.5f;
    float m_RotationSpeed = 50.0f;
};
