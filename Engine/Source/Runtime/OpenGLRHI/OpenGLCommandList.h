#pragma once

#include "OpenGLGraphicsPipeline.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexBuffer.h"
#include "RHICore/CommandList.h"

class COpenGLCommandList : public ICommandList
{
public:
    COpenGLCommandList() = default;
    ~COpenGLCommandList() override = default;

    void BeginRecording() override {}
    void EndRecording() override {}
    
    void SetGraphicsPipeline(const std::shared_ptr<IGraphicsPipeline>& GraphicsPipeline) override;
    void SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& VertexBuffer, uint32 Slot = 0) override;
    void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& IndexBuffer) override;
    void SetTexture(const std::shared_ptr<ITexture>& Texture, uint32 Slot = 0) override;
    void SetUniformBuffer(const std::shared_ptr<IUniformBuffer>& UniformBuffer) override;
    
    void DrawIndexed(uint32 IndexCount, uint32 InstanceCount = 1, uint32 FirstIndex = 0) override;
private:
    std::shared_ptr<COpenGLGraphicsPipeline> m_BoundPipeline = nullptr;
    std::shared_ptr<COpenGLVertexBuffer> m_BoundVertexBuffer = nullptr;
    std::shared_ptr<COpenGLIndexBuffer> m_BoundIndexBuffer = nullptr;
};
