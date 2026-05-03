#pragma once

#include "CoreDefines.h"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

struct MOONLIGHT_API CSpriteRendererComponent
{
    glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };

    CSpriteRendererComponent() = default;
    CSpriteRendererComponent(const CSpriteRendererComponent&) = default;
    CSpriteRendererComponent(const glm::vec4& Color)
        : Color(Color) {}
};

inline void to_json(nlohmann::ordered_json& Json, const CSpriteRendererComponent& SpriteRendererComponent)
{
    Json = nlohmann::ordered_json {
        {
            "Color", {
                { "Red", SpriteRendererComponent.Color.x },
                { "Green", SpriteRendererComponent.Color.y },
                { "Blue", SpriteRendererComponent.Color.z },
                { "Alpha", SpriteRendererComponent.Color.w }
            }
        }
    };
}

inline void from_json(const nlohmann::ordered_json& Json, CSpriteRendererComponent& SpriteRendererComponent)
{
    Json.at("Color").at("Red").get_to(SpriteRendererComponent.Color.x);
    Json.at("Color").at("Green").get_to(SpriteRendererComponent.Color.y);
    Json.at("Color").at("Blue").get_to(SpriteRendererComponent.Color.z);
    Json.at("Color").at("Alpha").get_to(SpriteRendererComponent.Color.w);
}
