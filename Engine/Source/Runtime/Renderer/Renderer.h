#pragma once

#include "RHICore/IndexBuffer.h"
#include "RHICore/RHIBackend.h"

enum class EGraphicsAPI
{
    OpenGL,
    Vulkan,
    Direct3D11,
    Direct3D12,
    Metal
};

struct FRHICommandPacket
{
    std::shared_ptr<FIndexBuffer> IndexBuffer;
};

class FRenderer
{
public:
    static void PreInitialize();
    static void Initialize();
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();

    static void DrawIndexedPrimitive(const FRHICommandPacket& RHICommandPacket);

    static void ClearColor(float Red, float Green, float Blue, float Alpha);
    
    static EGraphicsAPI GetGraphicsAPI() { return m_GraphicsAPI; }
    static void SetGraphicsAPI(EGraphicsAPI NewGraphicsAPI) { m_GraphicsAPI = NewGraphicsAPI; }
private:
    inline static std::shared_ptr<FRHIBackend> m_RHIBackend;
    
    inline static EGraphicsAPI m_GraphicsAPI;
};