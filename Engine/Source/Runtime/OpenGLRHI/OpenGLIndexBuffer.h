#pragma once

#include "RHICore/IndexBuffer.h"

class FOpenGLIndexBuffer : public FIndexBuffer
{
public:
    FOpenGLIndexBuffer(const void* BufferData, uint64 BufferSize);
    ~FOpenGLIndexBuffer() override;

    void Bind() const override;
private:
    uint32 m_BufferHandle = 0;
};
