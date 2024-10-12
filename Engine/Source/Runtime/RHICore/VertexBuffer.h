#pragma once

#include <memory>

enum class EVertexBufferUsage
{
    Static,
    Dynamic
};

class IVertexBuffer
{
public:
    virtual ~IVertexBuffer() = default;
    
    static std::shared_ptr<IVertexBuffer> Create(const void* Data, uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Static);
    static std::shared_ptr<IVertexBuffer> Create(uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Dynamic);
};
