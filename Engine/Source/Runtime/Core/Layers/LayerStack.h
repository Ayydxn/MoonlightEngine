#pragma once

#include "Layer.h"
#include "Assertion/AssertionMacros.h"

class MOONLIGHT_API CLayerStack
{
public:
    CLayerStack() = default;
    ~CLayerStack() = default;

    void PushLayer(CLayer* Layer);
    void PushOverlay(CLayer* Overlay);
    void PopLayer(CLayer* Layer);
    void PopOverlay(CLayer* Overlay);

    CLayer* operator[](size_t Index)
    {
        verifyEngine(Index >= 0 && Index < m_Layers.size())

        return m_Layers[Index];
    }

    CLayer* operator[](size_t Index) const
    {
        verifyEngine(Index >= 0 && Index < m_Layers.size())

        return m_Layers[Index];
    }

    size_t Size() const { return m_Layers.size(); }

    std::vector<CLayer*>::iterator begin() { return m_Layers.begin(); }
    std::vector<CLayer*>::iterator end() { return m_Layers.end(); }
private:
    std::vector<CLayer*> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};
