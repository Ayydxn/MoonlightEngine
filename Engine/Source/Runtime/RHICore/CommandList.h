#pragma once

#include "UniformBuffer.h"
#include "Renderer/Renderer.h"

class ICommandList
{
public:
    virtual ~ICommandList() = default;
    
    virtual void BeginRecording() = 0;
    virtual void EndRecording() = 0;
    
    virtual void SetGraphicsPipeline(const std::shared_ptr<IGraphicsPipeline>& GraphicsPipeline) = 0;
    virtual void SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& VertexBuffer, uint32 Slot = 0) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& IndexBuffer) = 0;
    virtual void SetTexture(const std::shared_ptr<ITexture>& Texture, uint32 Slot = 0) = 0;
    virtual void SetUniformBuffer(const std::shared_ptr<IUniformBuffer>& UniformBuffer) = 0;
    
    virtual void DrawIndexed(uint32 IndexCount, uint32 InstanceCount = 1, uint32 FirstIndex = 0) = 0;
};
