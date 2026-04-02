#pragma once

#include "RHICore/RHIFactory.h"

class COpenGLRHIFactory : public IRHIFactory
{
public:
    COpenGLRHIFactory() = default;
    ~COpenGLRHIFactory() override = default;
    
    std::shared_ptr<IRendererContext> CreateRendererContext(void* WindowHandle) override;
    std::shared_ptr<IRendererBackend> CreateRendererBackend() override;
    std::shared_ptr<IVertexBuffer> CreateVertexBuffer(uint64 Size, EVertexBufferUsage Usage) override;
    std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const void* Data, uint64 Size, EVertexBufferUsage Usage) override;
    std::shared_ptr<IIndexBuffer> CreateIndexBuffer(uint32* Indices, uint32 Count) override;
    std::shared_ptr<ITexture> CreateTexture(const CTextureSpecification& Specification) override;
    std::shared_ptr<IUniformBuffer> CreateUniformBuffer(uint64 Size, uint32 BindingPoint) override;
    std::shared_ptr<IShader> CreateShader(const std::filesystem::path& Filepath) override;
    std::shared_ptr<IGraphicsPipeline> CreateGraphicsPipeline(const CGraphicsPipelineSpecification& Specification) override;
    std::shared_ptr<IFramebuffer> CreateFramebuffer(const CFramebufferSpecification& Specification) override;
    std::shared_ptr<ICommandBuffer> CreateCommandBuffer() override;
    std::shared_ptr<ICommandList> CreateCommandList() override;
};
