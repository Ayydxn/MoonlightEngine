#pragma once

#include "RHICore/VertexBuffer.h"

class FOpenGLVertexBuffer : public FVertexBuffer 
{
public:
    FOpenGLVertexBuffer(const void* BufferData, uint64 BufferSize);
    ~FOpenGLVertexBuffer() override;

    void Bind() const override;
private:
    uint32 m_BufferHandle = 0;
};
