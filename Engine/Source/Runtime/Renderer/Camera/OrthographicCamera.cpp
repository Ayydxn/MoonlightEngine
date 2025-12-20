#include "MoonlightPCH.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

COrthographicCamera::COrthographicCamera(float Left, float Right, float Bottom, float Top)
    : m_ProjectionMatrix(glm::ortho(Left, Right, Bottom, Top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
{
    m_ViewProjectionMatrix  = m_ProjectionMatrix * m_ViewMatrix;
}

void COrthographicCamera::SetProjection(float Left, float Right, float Bottom, float Top)
{
    m_ProjectionMatrix = glm::ortho(Left, Right, Bottom, Top, -1.0f, 1.0f);
    m_ViewProjectionMatrix  = m_ProjectionMatrix * m_ViewMatrix;
}

void COrthographicCamera::RecalculateViewMatrix()
{
    const glm::mat4 CameraTransform = glm::translate(glm::mat4(1.0f), m_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix = glm::inverse(CameraTransform);
    m_ViewProjectionMatrix  = m_ProjectionMatrix * m_ViewMatrix;
}

