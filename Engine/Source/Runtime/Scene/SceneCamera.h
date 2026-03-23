#pragma once

#include "Renderer/Camera/Camera.h"

class CSceneCamera : public CCamera
{
public:
    CSceneCamera() = default;
    ~CSceneCamera() override = default;
    
    void SetOrthographicProjection(float Size, float NearClip, float FarClip);
    void SetPerspectiveProjection(float FOV, float NearClip, float FarClip);
    
    void SetViewportSize(uint32 ViewportWidth, uint32 ViewportHeight);
private:
    void RecalculateProjectionMatrix();
private:
    EProjectionType m_ProjectionType = EProjectionType::Perspective;
    float m_AspectRatio = 0.0f;
    
    float m_PerspectiveFOV = 90.0f;
    float m_PerspectiveNearClip = 0.1f;
    float m_PerspectiveFarClip = 1000.0f;
    
    float m_OrthographicSize = 10.0f;
    float m_OrthographicFarClip = 1.0f;
    float m_OrthographicNearClip = -1.0f;
};
