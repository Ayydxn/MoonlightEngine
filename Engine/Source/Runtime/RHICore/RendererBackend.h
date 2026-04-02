#pragma once

#include "GraphicsPipeline.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"

#include <memory>

struct FRenderPacket
{
    std::shared_ptr<IShader> Shader;
    std::shared_ptr<IVertexBuffer> VertexBuffer;
    std::shared_ptr<IIndexBuffer> IndexBuffer;
    std::shared_ptr<IGraphicsPipeline> GraphicsPipeline;
    std::shared_ptr<ITexture> Texture;
    std::shared_ptr<IUniformBuffer> UniformBuffer;

    void* UserData;
};

class IRendererBackend
{
public:
    virtual ~IRendererBackend() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void DrawIndexed(const FRenderPacket& RenderPacket, uint32 IndexCount) = 0;
};
