#include "MoonlightPCH.h"
#include "LayerStack.h"

void CLayerStack::PushLayer(CLayer* Layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, Layer);
    m_LayerInsertIndex++;

    Layer->OnAttach();
}

void CLayerStack::PushOverlay(CLayer* Overlay)
{
    m_Layers.emplace_back(Overlay);

    Overlay->OnAttach();
}

void CLayerStack::PopLayer(CLayer* Layer)
{
    if (const auto It = std::ranges::find(m_Layers, Layer); It != m_Layers.end())
    {
        m_Layers.erase(It);
        m_LayerInsertIndex--;
    }

    Layer->OnDetach();
}

void CLayerStack::PopOverlay(CLayer* Overlay)
{
    if (const auto It = std::ranges::find(m_Layers, Overlay); It != m_Layers.end())
        m_Layers.erase(It);

    Overlay->OnDetach();
}