#pragma once

#include "Scene/SceneCamera.h"

struct MOONLIGHT_API CCameraComponent
{
    CSceneCamera Camera;
    bool bIsPrimary = true;
    bool bUseFixedAspectRatio = false;
    
    CCameraComponent() = default;
    CCameraComponent(const CCameraComponent&) = default;
};