#pragma once

#include "CommandBuffer.h"
#include "CommandList.h"
#include "Framebuffer.h"
#include "GraphicsPipeline.h"
#include "IndexBuffer.h"
#include "RendererBackend.h"
#include "RendererContext.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"

#include <memory>

class IRHIFactory
{
public:
    virtual ~IRHIFactory() = default;
    
    virtual std::shared_ptr<IRendererContext> CreateRendererContext(void* WindowHandle) = 0;
    virtual std::shared_ptr<IRendererBackend> CreateRendererBackend() = 0;
    virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Dynamic) = 0;
    virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const void* Data, uint64 Size, EVertexBufferUsage Usage = EVertexBufferUsage::Static) =0;
    virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(uint32* Indices, uint32 Count) = 0;
    virtual std::shared_ptr<ITexture> CreateTexture(const CTextureSpecification& Specification) = 0;
    virtual std::shared_ptr<IUniformBuffer> CreateUniformBuffer(uint64 Size, uint32 BindingPoint) = 0;
    virtual std::shared_ptr<IShader> CreateShader(const std::filesystem::path& Filepath) = 0;
    virtual std::shared_ptr<IGraphicsPipeline> CreateGraphicsPipeline(const CGraphicsPipelineSpecification& Specification) = 0;
    virtual std::shared_ptr<IFramebuffer> CreateFramebuffer(const CFramebufferSpecification& Specification) = 0;
    virtual std::shared_ptr<ICommandBuffer> CreateCommandBuffer() = 0;
    virtual std::shared_ptr<ICommandList> CreateCommandList() = 0;

    static std::unique_ptr<IRHIFactory> Create();
};
