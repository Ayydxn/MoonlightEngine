#pragma once

class IUniformBuffer
{
public:
    virtual ~IUniformBuffer() = default;
    
    virtual void SetData(const void* Data, uint32_t Size, uint32_t Offset = 0) = 0;
};
