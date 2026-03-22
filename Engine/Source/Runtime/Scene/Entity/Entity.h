#pragma once

#include "Scene/Scene.h"

#include <entt/entt.hpp>

class MOONLIGHT_API CEntity
{
public:
    CEntity(entt::entity Handle, CScene* Scene);
    
    template<typename T, typename... Args>
    T& AddComponent(Args&& ... Arguments)
    {
        assertEnginef(!HasComponent<T>(), "Entity already has that component!")
        
        return m_Scene->m_EntityRegistry.emplace<T>(m_Handle, std::forward<Args>(Arguments)...);
    }
    
    template<typename T>
    void RemoveComponent()
    {
        assertEnginef(HasComponent<T>(), "Entity does not have that component!")
        
        m_Scene->m_EntityRegistry.remove<T>(m_Handle);
    }
    
    template<typename T>
    T& GetComponent()
    {
        assertEnginef(HasComponent<T>(), "Entity does not have that component!")
        
        return m_Scene->m_EntityRegistry.get<T>(m_Handle);
    }
    
    template<typename T>
    bool HasComponent() const
    {
        return m_Scene->m_EntityRegistry.all_of<T>(m_Handle);
    }
    
    operator bool() { return m_Handle != entt::null; }
    operator bool() const { return m_Handle != entt::null; }
private:
    CScene* m_Scene;
    
    entt::entity m_Handle = entt::null;
};