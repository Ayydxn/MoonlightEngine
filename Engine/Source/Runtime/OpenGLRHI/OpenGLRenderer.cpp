#include "MoonlightPCH.h"
#include "OpenGLRenderer.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <tracy/TracyOpenGL.hpp>

void COpenGLRenderer::Initialize()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_CommandList = Cast<COpenGLCommandList>(ICommandList::Create());
}

void COpenGLRenderer::Shutdown()
{
    m_CommandList.reset();
}

void COpenGLRenderer::BeginFrame()
{
    constexpr float ClearColor[4] = { 0.15f, 0.15f, 0.15f, 1.0f };

    glClearBufferfv(GL_COLOR, 0, ClearColor);
    glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
}

void COpenGLRenderer::EndFrame()
{
}

void COpenGLRenderer::DrawIndexed(const CRenderPacket& RenderPacket, const glm::mat4& Transform, uint32 IndexCount)
{
    Cast<COpenGLShader>(RenderPacket.Shader)->Bind();
    
    m_CommandList->SetGraphicsPipeline(RenderPacket.GraphicsPipeline);
    m_CommandList->SetVertexBuffer(RenderPacket.VertexBuffer);
    m_CommandList->SetIndexBuffer(RenderPacket.IndexBuffer);
    
    if (RenderPacket.Texture)
        m_CommandList->SetTexture(RenderPacket.Texture);
    
    if (RenderPacket.UniformBuffer)
        m_CommandList->SetUniformBuffer(RenderPacket.UniformBuffer);
    
    m_CommandList->DrawIndexed(IndexCount);
}
