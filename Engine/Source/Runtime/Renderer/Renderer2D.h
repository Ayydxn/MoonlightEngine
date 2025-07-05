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
    static void DrawQuad(const glm::vec2& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    static void DrawQuad(const glm::vec3& Position, const glm::vec2& Size, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);

    static void DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color);
    static void DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const glm::vec4& Color);
    static void DrawRotatedQuad(const glm::vec2& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
    static void DrawRotatedQuad(const glm::vec3& Position, const glm::vec2& Size, float Rotation, const std::shared_ptr<ITexture>& Texture, float TilingFactor = 1.0f);
};
