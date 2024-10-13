#pragma once

#include "GraphicsPipeline.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include <memory>

#include "Texture.h"

struct CRenderPacket
{
    std::shared_ptr<IShader> Shader;
    std::shared_ptr<IVertexBuffer> VertexBuffer;
    std::shared_ptr<IIndexBuffer> IndexBuffer;
    std::shared_ptr<IGraphicsPipeline> GraphicsPipeline;
    std::shared_ptr<ITexture> Texture;
};

class IRendererBackend
{
public:
    virtual ~IRendererBackend() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void DrawIndexed(const CRenderPacket& RenderPacket) = 0;
    
    static std::shared_ptr<IRendererBackend> Create();
};
