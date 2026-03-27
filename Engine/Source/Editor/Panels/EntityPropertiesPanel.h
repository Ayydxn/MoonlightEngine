#pragma once

#include "Scene/Entity/Entity.h"

class CEntityPropertiesPanel
{
public:
    CEntityPropertiesPanel() = default;
    CEntityPropertiesPanel(CEntity Entity);
    
    void OnImGuiRender();
    
    void ClearEntity() { m_Entity = {}; }
    
    void SetEntity(const CEntity Entity) { m_Entity = Entity; }
private:
    void DrawEntityComponents();
    
    template<typename T, typename DrawFunction>
    void DrawComponent(const std::string& Name, DrawFunction UIDrawFunction);
private:
    CEntity m_Entity;
};
