#pragma once

#include <memory>

struct FRHICommandPacket;

class FRHIBackend
{
public:
    virtual ~FRHIBackend() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void DrawIndexedPrimitive(const FRHICommandPacket& RHICommandPacket) = 0;

    virtual void ClearColor(float Red, float Green, float Blue, float Alpha) = 0;

    static std::shared_ptr<FRHIBackend> Create();
};
