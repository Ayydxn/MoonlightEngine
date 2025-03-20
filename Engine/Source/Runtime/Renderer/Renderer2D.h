#pragma once

#include "CoreDefines.h"
#include "RHICore/Texture.h"

#include <glm/glm.hpp>

class MOONLIGHT_API CRenderer2D
{
public:
    static void Initialize();
    static void Shutdown();
    
    static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const glm::vec4& Color);
    static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const glm::vec4& Color);

    static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture);
    static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture);
};
