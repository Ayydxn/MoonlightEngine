#pragma once

#include <memory>

class FVertexBuffer
{
public:
    virtual ~FVertexBuffer() = default;

    virtual void Bind() const = 0;

    static std::shared_ptr<FVertexBuffer> Create(const void* BufferData, uint64 BufferSize);
};
