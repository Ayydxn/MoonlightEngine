#pragma once

#include "Layers/Layer.h"

class IImGuiLayer : public CLayer
{
public:
    IImGuiLayer();
    ~IImGuiLayer() override = default;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    
    void OnEvent(IEvent& Event) override;
    
    void BlockEvents(bool bEnableEventBlocking) { bBlockingEvents = bEnableEventBlocking; }

    void SetDarkThemeColors();
    
    static IImGuiLayer* Create();
private:
    bool bBlockingEvents = false;
};
