#pragma once

#include "CoreDefines.h"
#include "Events/Event.h"

#include <string>

class MOONLIGHT_API CLayer
{
public:
    CLayer(const std::string& Name = "Layer");
    virtual ~CLayer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
    virtual void OnPostRender() {}
    virtual void OnTick() {}
    virtual void OnEvent(IEvent& Event) {}

    const std::string& GetName() const { return m_LayerName; }
protected:
    std::string m_LayerName;
};
