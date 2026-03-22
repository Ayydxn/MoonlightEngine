#include "MoonlightPCH.h"
#include "Entity.h"

CEntity::CEntity(entt::entity Handle, CScene* Scene)
    : m_Handle(Handle), m_Scene(Scene) {}
