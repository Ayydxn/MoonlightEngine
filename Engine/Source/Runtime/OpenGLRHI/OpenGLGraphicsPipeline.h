#pragma once

#include "RHICore/GraphicsPipeline.h"

class FOpenGLGraphicsPipeline : public FGraphicsPipeline
{
public:
    FOpenGLGraphicsPipeline();
    ~FOpenGLGraphicsPipeline() override;

    void Invalidate() override;
    void Bind() override;
private:
    uint32 GetShaderDataTypeGLDataType(EShaderDataType ShaderDataType);
private:
    uint32 m_VertexArrayHandle = 0;
};
