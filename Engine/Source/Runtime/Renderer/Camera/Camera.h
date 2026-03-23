#pragma once

#include <glm/glm.hpp>

enum class EProjectionType
{
    Perspective,
    Orthographic
};

class MOONLIGHT_API CCamera
{
public:
    CCamera() = default;
    CCamera(const glm::mat4& ProjectionMatrix)
        : m_ProjectionMatrix(ProjectionMatrix) {}
    virtual ~CCamera() = default;
    
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
 protected:
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
};
