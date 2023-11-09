#include "MoonlightPCH.h"
#include "LayerStack.h"

void FLayerStack::PushLayer(FLayer* Layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, Layer);
    m_LayerInsertIndex++;

    Layer->OnAttach();
}

void FLayerStack::PushOverlay(FLayer* Overlay)
{
    m_Layers.emplace_back(Overlay);

    Overlay->OnAttach();
}

void FLayerStack::PopLayer(FLayer* Layer)
{
    auto It = std::ranges::find(m_Layers, Layer);
    if (It != m_Layers.end())
    {
        m_Layers.erase(It);
        m_LayerInsertIndex--;
    }

    Layer->OnDetach();
}

void FLayerStack::PopOverlay(FLayer* Overlay)
{
    auto It = std::ranges::find(m_Layers, Overlay);
    if (It != m_Layers.end())
        m_Layers.erase(It);

    Overlay->OnDetach();
}