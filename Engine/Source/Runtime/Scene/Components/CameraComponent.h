#pragma once

#include "Scene/SceneCamera.h"

#include <nlohmann/json.hpp>
#include <nlohmann/detail/macro_scope.hpp>

struct MOONLIGHT_API CCameraComponent
{
    CSceneCamera Camera;
    bool bIsPrimary = true;
    bool bUseFixedAspectRatio = false;
    
    CCameraComponent() = default;
    CCameraComponent(const CCameraComponent&) = default;
};

NLOHMANN_JSON_SERIALIZE_ENUM(EProjectionType, {
    { EProjectionType::Perspective, "Perspective" },
    { EProjectionType::Orthographic, "Orthographic" }
});

inline void to_json(nlohmann::ordered_json& Json, const CCameraComponent& CameraComponent)
{
    Json = nlohmann::ordered_json {
        { "ProjectionType", CameraComponent.Camera.GetProjectionType() },
        { "PerspectiveFOV", CameraComponent.Camera.GetPerspectiveFOV() },
        { "PerspectiveNearClip", CameraComponent.Camera.GetPerspectiveNearClip() },
        { "PerspectiveFarClip", CameraComponent.Camera.GetPerspectiveFarClip() },
        { "OrthographicSize", CameraComponent.Camera.GetOrthographicSize() },
        { "OrthographicFarClip", CameraComponent.Camera.GetOrthographicFarClip() },
        { "OrthographicNearClip", CameraComponent.Camera.GetOrthographicNearClip() },
        { "IsPrimary", CameraComponent.bIsPrimary },
        { "UseFixedAspectRatio", CameraComponent.bUseFixedAspectRatio }
    };
}

inline void from_json(const nlohmann::ordered_json& Json, CCameraComponent& CameraComponent)
{
    CameraComponent.Camera.SetProjectionType(Json.at("ProjectionType").get<EProjectionType>());
    CameraComponent.Camera.SetPerspectiveFOV(Json.at("PerspectiveFOV").get<float>());
    CameraComponent.Camera.SetPerspectiveNearClip(Json.at("PerspectiveNearClip").get<float>());
    CameraComponent.Camera.SetPerspectiveFarClip(Json.at("PerspectiveFarClip").get<float>());
    CameraComponent.Camera.SetOrthographicSize(Json.at("OrthographicSize").get<float>());
    CameraComponent.Camera.SetOrthographicFarClip(Json.at("OrthographicFarClip").get<float>());
    CameraComponent.Camera.SetOrthographicNearClip(Json.at("OrthographicNearClip").get<float>());
    
    Json.at("IsPrimary").get_to(CameraComponent.bIsPrimary);
    Json.at("UseFixedAspectRatio").get_to(CameraComponent.bUseFixedAspectRatio);
}