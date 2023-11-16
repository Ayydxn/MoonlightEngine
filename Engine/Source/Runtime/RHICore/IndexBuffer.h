#pragma once

#include <memory>

class FIndexBuffer
{
public:
    virtual ~FIndexBuffer() = default;

    virtual void Bind() const = 0;

    static std::shared_ptr<FIndexBuffer> Create(const void* BufferData, uint64 BufferSize);
};
