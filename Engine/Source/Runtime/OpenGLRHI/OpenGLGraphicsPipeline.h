#pragma once

#include "RHICore/GraphicsPipeline.h"

class COpenGLGraphicsPipeline : public IGraphicsPipeline
{
public:
    COpenGLGraphicsPipeline();
    ~COpenGLGraphicsPipeline() override;

    void Invalidate() override;

    void Bind() const;

    uint32 GetStride() const { return m_Stride; }
    
    uint32 GetHandle() const { return m_VertexArrayHandle; }
private:
    uint32 m_Stride = 0;
    uint32 m_VertexArrayHandle = 0;
};
