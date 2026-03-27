#pragma once

#include "EntityPropertiesPanel.h"
#include "Scene/Scene.h"
#include "Scene/Entity/Entity.h"

class MOONLIGHT_API CSceneHierarchyPanel
{
public:
    CSceneHierarchyPanel() = default;
    CSceneHierarchyPanel(const std::shared_ptr<CScene>& Scene);
    
    void SetContext(const std::shared_ptr<CScene>& Scene) { m_Scene = Scene; }
    
    void OnImGuiRender();
private:
    void DrawEntityNode(CEntity Entity);
private:
    std::shared_ptr<CScene> m_Scene;
    
    CEntityPropertiesPanel m_PropertiesPanel;
    
    CEntity m_SelectedEntity;
};
