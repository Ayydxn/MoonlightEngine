#pragma once

#include "Layer.h"
#include "Core/Misc/AssertionMacros.h"

class FLayerStack
{
public:
    FLayerStack() = default;
    ~FLayerStack() = default;

    void PushLayer(FLayer* Layer);
    void PushOverlay(FLayer* Overlay);
    void PopLayer(FLayer* Layer);
    void PopOverlay(FLayer* Overlay);

    FLayer* operator[](size_t Index)
    {
        verifyEngine(Index >= 0 && Index < m_Layers.size())

        return m_Layers[Index];
    }

    FLayer* operator[](size_t Index) const
    {
        verifyEngine(Index >= 0 && Index < m_Layers.size())

        return m_Layers[Index];
    }

    size_t Size() const { return m_Layers.size(); }

    std::vector<FLayer*>::iterator begin() { return m_Layers.begin(); }
    std::vector<FLayer*>::iterator end() { return m_Layers.end(); }
private:
    std::vector<FLayer*> m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};
