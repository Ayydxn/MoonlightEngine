#include "MoonlightPCH.h"
#include "OpenGLRenderer.h"
#include "OpenGLGraphicsPipeline.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

void COpenGLRenderer::Initialize()
{
}

void COpenGLRenderer::Shutdown()
{
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

void COpenGLRenderer::DrawIndexed(const CRenderPacket& RenderPacket)
{
    verifyEnginef(RenderPacket.Shader, "{} requires a non-null shader!", __FUNCTION__)
    verifyEnginef(RenderPacket.VertexBuffer, "{} requires a non-null vertex buffer!", __FUNCTION__)
    verifyEnginef(RenderPacket.IndexBuffer, "{} requires a non-null index buffer!", __FUNCTION__)
    verifyEnginef(RenderPacket.GraphicsPipeline, "{} requires a non-null graphics pipeline!", __FUNCTION__)

    const auto Shader = Cast<COpenGLShader>(RenderPacket.Shader);
    const auto VertexBuffer = Cast<COpenGLVertexBuffer>(RenderPacket.VertexBuffer);
    const auto IndexBuffer = Cast<COpenGLIndexBuffer>(RenderPacket.IndexBuffer);
    const auto VertexArray = Cast<COpenGLGraphicsPipeline>(RenderPacket.GraphicsPipeline);
    
    Shader->Bind();
    VertexArray->Bind();

    glVertexArrayVertexBuffer(VertexArray->GetHandle(), 0, VertexBuffer->GetHandle(), 0, static_cast<int32>(VertexArray->GetStride()));
    glVertexArrayElementBuffer(VertexArray->GetHandle(), IndexBuffer->GetHandle());
    
    glDrawElements(GL_TRIANGLES, static_cast<int32>(IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr);
}
