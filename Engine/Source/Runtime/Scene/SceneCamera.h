#pragma once

#include "Renderer/Camera/Camera.h"

class CSceneCamera : public CCamera
{
public:
    CSceneCamera() = default;
    ~CSceneCamera() override = default;
    
    EProjectionType GetProjectionType() const { return m_ProjectionType; }
    void SetProjectionType(const EProjectionType NewProjectionType) { m_ProjectionType = NewProjectionType; RecalculateProjectionMatrix(); }
    
    void SetOrthographicProjection(float Size, float NearClip, float FarClip);
    void SetPerspectiveProjection(float FOV, float NearClip, float FarClip);
    
    float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
    void SetPerspectiveFOV(const float FOV) { m_PerspectiveFOV = FOV; RecalculateProjectionMatrix();}
    
    float GetPerspectiveNearClip() const { return m_PerspectiveNearClip; }
    void SetPerspectiveNearClip(const float NearClip) { m_PerspectiveNearClip = NearClip; RecalculateProjectionMatrix(); }
    
    float GetPerspectiveFarClip() const { return m_PerspectiveFarClip; }
    void SetPerspectiveFarClip(const float FarClip) { m_PerspectiveFarClip = FarClip; RecalculateProjectionMatrix(); }
    
    float GetOrthographicSize() const { return m_OrthographicSize; }
    void SetOrthographicSize(const float Size) { m_OrthographicSize = Size; RecalculateProjectionMatrix(); }
    
    float GetOrthographicFarClip() const { return m_OrthographicFarClip; }
    void SetOrthographicFarClip(const float FarClip) { m_OrthographicFarClip = FarClip; RecalculateProjectionMatrix(); }
    
    float GetOrthographicNearClip() const { return m_OrthographicNearClip; }
    void SetOrthographicNearClip(const float NearClip) { m_OrthographicNearClip = NearClip; RecalculateProjectionMatrix(); }
    
    void SetViewportSize(uint32 ViewportWidth, uint32 ViewportHeight);
private:
    void RecalculateProjectionMatrix();
private:
    EProjectionType m_ProjectionType = EProjectionType::Perspective;
    float m_AspectRatio = 0.0f;
    
    float m_PerspectiveFOV = glm::radians(90.0f);
    float m_PerspectiveNearClip = 0.1f;
    float m_PerspectiveFarClip = 1000.0f;
    
    float m_OrthographicSize = 10.0f;
    float m_OrthographicFarClip = 1.0f;
    float m_OrthographicNearClip = -1.0f;
};
