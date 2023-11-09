#pragma once

#include "Events/Event.h"

class FLayer
{
public:
    FLayer(const std::string& Name = "Layer");
    virtual ~FLayer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnPostRender() {}
    virtual void OnTick() {}
    virtual void OnEvent(FEvent& Event) {}

    const std::string& GetName() const { return LayerName; }
protected:
    std::string LayerName;
};
