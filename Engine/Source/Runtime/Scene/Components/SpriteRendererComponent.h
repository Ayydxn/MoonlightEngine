#pragma once

#include "CoreDefines.h"

#include <glm/glm.hpp>

struct MOONLIGHT_API CSpriteRendererComponent
{
    glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    CSpriteRendererComponent() = default;
    CSpriteRendererComponent(const CSpriteRendererComponent&) = default;
    CSpriteRendererComponent(const glm::vec4& Color)
        : Color(Color) {}
};
