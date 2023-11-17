#include "MoonlightPCH.h"
#include "VertexBufferLayout.h"

void FVertexBufferLayout::CalculateOffsetsAndStride()
{
    uint32 Offset = 0;
    m_Stride = 0;

    for (auto& VertexBufferElement : m_VertexBufferElements)
    {
        VertexBufferElement.Offset = Offset;
        Offset += VertexBufferElement.Size;
        m_Stride += VertexBufferElement.Size;
    }
}
