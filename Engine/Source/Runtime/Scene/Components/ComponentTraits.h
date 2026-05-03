#pragma once

#include "CameraComponent.h"
#include "SpriteRendererComponent.h"
#include "TagComponent.h"
#include "TransformComponent.h"

template <typename T>
const char* GetComponentName()
{
    // Fallback
    return typeid(T).name();
}

#define REGISTER_COMPONENT_NAME(Type, Name) template<> inline const char* GetComponentName<Type>() { return Name; }

REGISTER_COMPONENT_NAME(CTransformComponent, "Transform")
REGISTER_COMPONENT_NAME(CTagComponent, "Tag")
REGISTER_COMPONENT_NAME(CSpriteRendererComponent, "Sprite Renderer")
REGISTER_COMPONENT_NAME(CCameraComponent, "Camera")