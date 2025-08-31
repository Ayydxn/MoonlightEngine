#pragma once

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

    void DrawIndexed(const CRenderPacket& RenderPacket, const glm::mat4& Transform, uint32 IndexCount, const CSceneData* SceneData) override;
};
