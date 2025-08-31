#pragma once

#include "RHICore/VertexBuffer.h"

class COpenGLVertexBuffer : public IVertexBuffer
{
public:
    COpenGLVertexBuffer(const void* Data, uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Static);
    COpenGLVertexBuffer(uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Dynamic);
    ~COpenGLVertexBuffer() override;

    void SetData(const void* Data, uint64 Size) const override;

    uint64 GetSize() const override { return m_Size; }

    uint32 GetHandle() const { return m_VertexBufferHandle; } 
private:
    uint64 m_Size = 0;
    uint32 m_VertexBufferHandle = 0;
};
