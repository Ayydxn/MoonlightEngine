#include "MoonlightPCH.h"
#include "OpenGLRHIFactory.h"
#include "OpenGLCommandList.h"
#include "OpenGLContext.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLGraphicsPipeline.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLVertexBuffer.h"

std::shared_ptr<IRendererContext> COpenGLRHIFactory::CreateRendererContext(void* WindowHandle)
{
    return std::make_shared<COpenGLContext>(WindowHandle);
}

std::shared_ptr<IRendererBackend> COpenGLRHIFactory::CreateRendererBackend()
{
    return std::make_shared<COpenGLRenderer>();
}

std::shared_ptr<IVertexBuffer> COpenGLRHIFactory::CreateVertexBuffer(uint64 Size, EVertexBufferUsage Usage)
{
    return std::make_shared<COpenGLVertexBuffer>(Size, Usage);
}

std::shared_ptr<IVertexBuffer> COpenGLRHIFactory::CreateVertexBuffer(const void* Data, uint64 Size, EVertexBufferUsage Usage)
{
    return std::make_shared<COpenGLVertexBuffer>(Data, Size, Usage);
}

std::shared_ptr<IIndexBuffer> COpenGLRHIFactory::CreateIndexBuffer(uint32* Indices, uint32 Count)
{
    return std::make_shared<COpenGLIndexBuffer>(Indices, Count);
}

std::shared_ptr<ITexture> COpenGLRHIFactory::CreateTexture(const CTextureSpecification& Specification)
{
    return std::make_shared<COpenGLTexture>(Specification);
}

std::shared_ptr<IUniformBuffer> COpenGLRHIFactory::CreateUniformBuffer(uint64 Size, uint32 BindingPoint)
{
    return std::make_shared<COpenGLUniformBuffer>(Size, BindingPoint);
}

std::shared_ptr<IShader> COpenGLRHIFactory::CreateShader(const std::filesystem::path& Filepath)
{
    return std::make_shared<COpenGLShader>(Filepath);
}

std::shared_ptr<IGraphicsPipeline> COpenGLRHIFactory::CreateGraphicsPipeline(const CGraphicsPipelineSpecification& Specification)
{
    return std::make_shared<COpenGLGraphicsPipeline>(Specification);
}

std::shared_ptr<IFramebuffer> COpenGLRHIFactory::CreateFramebuffer(const CFramebufferSpecification& Specification)
{
    return std::make_shared<COpenGLFramebuffer>(Specification);
}

std::shared_ptr<ICommandBuffer> COpenGLRHIFactory::CreateCommandBuffer()
{
    // TODO: (Ayydxn) Return an OpenGL implementation which just no-ops everything. The concept of command buffers isn't a thing in OpenGL.
    return nullptr;
}

std::shared_ptr<ICommandList> COpenGLRHIFactory::CreateCommandList()
{
    return std::make_shared<COpenGLCommandList>();
}
