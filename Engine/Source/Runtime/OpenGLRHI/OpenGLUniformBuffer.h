#pragma once

#include "RHICore/UniformBuffer.h"

class COpenGLUniformBuffer : public IUniformBuffer
{
public:
    COpenGLUniformBuffer(uint32 Size, uint32_t Binding);
    ~COpenGLUniformBuffer() override;
    
    void SetData(const void* Data, uint32_t Size, uint32_t Offset = 0) override;
private:
    uint32 m_UniformBufferHandle;
};
