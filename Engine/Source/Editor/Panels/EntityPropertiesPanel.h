#pragma once

#include "Scene/Entity/Entity.h"

struct CComponentDrawOptions
{
    bool bNestInTreeNode = true;
    bool bIsRemovable = true;
};

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
    void DrawAddComponentMenu();
    
    template<typename T, typename DrawFunction>
    void DrawComponent(const std::string& Name, DrawFunction UIDrawFunction, CComponentDrawOptions DrawOptions = {});
private:
    CEntity m_Entity;
    
    // Used by the tag component's draw logic
    char m_NewTagBuffer[128] = {};
    char m_DuplicateTagName[128] = {};
    bool bOpenDuplicatePopup = false;
};
