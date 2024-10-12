#pragma once

#include "RHICore/GraphicsPipeline.h"

class COpenGLGraphicsPipeline : public IGraphicsPipeline
{
public:
    COpenGLGraphicsPipeline();
    ~COpenGLGraphicsPipeline() override;

    void Invalidate() override;
private:
    uint32 m_VertexArrayHandle = 0;
};
