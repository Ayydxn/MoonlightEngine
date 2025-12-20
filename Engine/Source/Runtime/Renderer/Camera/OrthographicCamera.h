#pragma once

#include "Core/CoreDefines.h"

#include <glm/glm.hpp>

class MOONLIGHT_API COrthographicCamera
{
public:
    COrthographicCamera(float Left, float Right, float Bottom, float Top);

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    
    void SetProjection(float Left, float Right, float Bottom, float Top);

    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& Position) { m_Position = Position; RecalculateViewMatrix(); }

    float GetRotation() const { return m_Rotation; }
    void SetRotation(float Rotation) { m_Rotation = Rotation; RecalculateViewMatrix(); }
private:
    void RecalculateViewMatrix();
private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_Rotation = 0.0f;
};
