#pragma once

#include "RHICore/RHIBackend.h"

class FOpenGLRenderer : public FRHIBackend
{
public:
    FOpenGLRenderer() = default;
    ~FOpenGLRenderer() override = default;

    void Initialize() override;
    void Shutdown() override;
    void BeginFrame() override;
    void EndFrame() override;

    void DrawIndexedPrimitive(const FRHICommandPacket& RHICommandPacket) override;

    void ClearColor(float Red, float Green, float Blue, float Alpha) override;
private:
    static void DebugMessageCallback(uint32 Source, uint32 Type, uint32 ID, uint32 Severity, int32 Length, const char* Message, const void* UserData);
};
