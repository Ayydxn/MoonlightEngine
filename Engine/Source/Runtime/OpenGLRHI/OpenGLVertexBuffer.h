#pragma once

#include "RHICore/VertexBuffer.h"

class COpenGLVertexBuffer : public IVertexBuffer
{
public:
    COpenGLVertexBuffer(const void* Data, uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Static);
    COpenGLVertexBuffer(uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Dynamic);
    ~COpenGLVertexBuffer() override;
private:
    uint32 m_VertexBufferHandle = 0;
};
