#pragma once

#include "OpenGLCommandList.h"
#include "RHICore/RendererBackend.h"

class COpenGLRenderer : public IRendererBackend
{
public:
    COpenGLRenderer() = default;
    ~COpenGLRenderer() override = default;

    void Initialize() override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    void DrawIndexed(const FRenderPacket& RenderPacket, uint32 IndexCount) override;
private:
    std::shared_ptr<COpenGLCommandList> m_CommandList;
};
