#include "MoonlightPCH.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

void CSceneCamera::SetOrthographicProjection(float Size, float NearClip, float FarClip)
{
    m_ProjectionType = EProjectionType::Orthographic;
    m_OrthographicSize = Size;
    m_OrthographicNearClip = NearClip;
    m_OrthographicFarClip = FarClip;
    
    RecalculateProjectionMatrix();
}

void CSceneCamera::SetPerspectiveProjection(float FOV, float NearClip, float FarClip)
{
    m_ProjectionType = EProjectionType::Perspective;
    m_PerspectiveFOV = FOV;
    m_PerspectiveNearClip = NearClip;
    m_PerspectiveFarClip = FarClip;
    
    RecalculateProjectionMatrix();
}

void CSceneCamera::SetViewportSize(uint32 ViewportWidth, uint32 ViewportHeight)
{
    m_AspectRatio = static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight);
    
    RecalculateProjectionMatrix();
}

void CSceneCamera::RecalculateProjectionMatrix()
{
    if (m_ProjectionType == EProjectionType::Perspective)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNearClip,
            m_PerspectiveFarClip);
    }
    else if (m_ProjectionType == EProjectionType::Orthographic)
    {
        const float Left = -m_OrthographicSize * m_AspectRatio * 0.5f;
        const float Right = m_OrthographicSize * m_AspectRatio * 0.5f;
        const float Bottom = -m_OrthographicSize * 0.5f;
        const float Top = m_OrthographicSize * 0.5f;
        
        m_ProjectionMatrix = glm::ortho(Left, Right, Bottom, Top, m_OrthographicNearClip, m_OrthographicFarClip);
    }
}
