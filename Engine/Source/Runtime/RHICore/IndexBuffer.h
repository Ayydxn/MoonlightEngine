#pragma once

#include <memory>

class FIndexBuffer
{
public:
    virtual ~FIndexBuffer() = default;

    virtual void Bind() const = 0;
    uint32 GetCount() const { return m_Size / sizeof(uint32); }

    static std::shared_ptr<FIndexBuffer> Create(const void* BufferData, uint64 BufferSize);
protected:
    inline static uint32 m_Size = 0;
};
