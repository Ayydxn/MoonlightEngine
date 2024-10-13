#pragma once

#include "RHICore/IndexBuffer.h"

class COpenGLIndexBuffer : public IIndexBuffer
{
public:
    COpenGLIndexBuffer(const void* Data, uint64 Size);
    ~COpenGLIndexBuffer() override;

    uint32 GetCount() const override { return m_Count; }

    uint32 GetHandle() const { return m_IndexBufferHandle; } 
private:
    uint32 m_Size = 0;
    uint32 m_Count = 0;
    uint32 m_IndexBufferHandle = 0;
};
