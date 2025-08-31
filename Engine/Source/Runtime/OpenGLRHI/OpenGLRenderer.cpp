#include "MoonlightPCH.h"
#include "OpenGLRenderer.h"
#include "OpenGLGraphicsPipeline.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexBuffer.h"
#include "Debug/Profiler.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera/OrthographicCamera.h"

#include <glad/glad.h>
#include <tracy/TracyOpenGL.hpp>

void COpenGLRenderer::Initialize()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void COpenGLRenderer::DrawIndexed(const CRenderPacket& RenderPacket, const glm::mat4& Transform, uint32 IndexCount, const CSceneData* SceneData)
{
    MOONLIGHT_PROFILE_GPU("DrawIndexed")
    
    verifyEnginef(RenderPacket.Shader, "{} requires a non-null shader!", __FUNCTION__)
    verifyEnginef(RenderPacket.VertexBuffer, "{} requires a non-null vertex buffer!", __FUNCTION__)
    verifyEnginef(RenderPacket.IndexBuffer, "{} requires a non-null index buffer!", __FUNCTION__)
    verifyEnginef(RenderPacket.GraphicsPipeline, "{} requires a non-null graphics pipeline!", __FUNCTION__)

    const auto Shader = Cast<COpenGLShader>(RenderPacket.Shader);
    const auto VertexBuffer = Cast<COpenGLVertexBuffer>(RenderPacket.VertexBuffer);
    const auto IndexBuffer = Cast<COpenGLIndexBuffer>(RenderPacket.IndexBuffer);
    const auto VertexArray = Cast<COpenGLGraphicsPipeline>(RenderPacket.GraphicsPipeline);
    
    Shader->Bind();
    Shader->SetMatrix4x4f("u_ViewProjectionMatrix", SceneData->ViewProjectionMatrix);
    Shader->SetMatrix4x4f("u_Transform", Transform);
    
    VertexArray->Bind();

    glVertexArrayVertexBuffer(VertexArray->GetHandle(), 0, VertexBuffer->GetHandle(), 0, static_cast<int32>(VertexArray->GetStride()));
    glVertexArrayElementBuffer(VertexArray->GetHandle(), IndexBuffer->GetHandle());

    if (RenderPacket.Texture != nullptr)
    {
        Shader->SetInt("u_Texture", 0);
        
        Cast<COpenGLTexture>(RenderPacket.Texture)->Bind();
    }
    
    const int32 Count = IndexCount == 0 ? static_cast<int32>(IndexBuffer->GetCount()) : static_cast<int32>(IndexCount); 
    glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, nullptr);
}
