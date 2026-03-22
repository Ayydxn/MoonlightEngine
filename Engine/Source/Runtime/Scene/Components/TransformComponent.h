#pragma once

#include "CoreDefines.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct MOONLIGHT_API CTransformComponent
{
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
    
    CTransformComponent() = default;
    CTransformComponent(const CTransformComponent&) = default;
    CTransformComponent(const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale)
        : Position(Position), Rotation(Rotation), Scale(Scale) {}
    
    glm::mat4 GetTransformMatrix() const
    {
        return glm::translate(glm::mat4(1.0f), Position) *
            glm::toMat4(glm::quat(Rotation)) *
            glm::scale(glm::mat4(1.0f), Scale);
    }
};
