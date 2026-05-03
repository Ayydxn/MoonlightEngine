#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace glm
{
    inline void to_json(nlohmann::ordered_json& Json, const vec3& Vec3)
    {
        Json = nlohmann::json {
            { "X", Vec3.x },
            { "Y", Vec3.y },
            { "Z", Vec3.z }
        };
    }

    inline void from_json(const nlohmann::ordered_json& Json, vec3& Vec3)
    {
        Json.at("X").get_to(Vec3.x);
        Json.at("Y").get_to(Vec3.y);
        Json.at("Z").get_to(Vec3.z);
    }

    inline void to_json(nlohmann::ordered_json& Json, const vec4& Vec4)
    {
        Json = nlohmann::json {
                { "X", Vec4.x },
                { "Y", Vec4.y },
                { "Z", Vec4.z },
                { "W", Vec4.w }
            };
    }

    inline void from_json(const nlohmann::ordered_json& Json, vec4& Vec4)
    {
        Json.at("X").get_to(Vec4.x);
        Json.at("Y").get_to(Vec4.y);
        Json.at("Z").get_to(Vec4.z);
        Json.at("W").get_to(Vec4.w);
    }
}
