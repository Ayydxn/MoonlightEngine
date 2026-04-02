#pragma once

#include <memory>

class ICommandBuffer
{
public:
    virtual ~ICommandBuffer() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;
    virtual void Submit() = 0;

    virtual void* GetActiveCommandBuffer() const = 0;
};
