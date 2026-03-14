#include "MoonlightPCH.h"
#include "OpenGLCommandList.h"
#include "OpenGLGraphicsPipeline.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexBuffer.h"
#include "Debug/Profiler.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include <tracy/TracyOpenGL.hpp>

#include "OpenGLUniformBuffer.h"

void COpenGLCommandList::SetGraphicsPipeline(const std::shared_ptr<IGraphicsPipeline>& GraphicsPipeline)
{
    verifyEnginef(GraphicsPipeline, "{} requires a non-null graphics pipeline!", __FUNCTION__)

    m_BoundPipeline = Cast<COpenGLGraphicsPipeline>(GraphicsPipeline);
    m_BoundPipeline->Bind();
}

void COpenGLCommandList::SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& VertexBuffer, uint32 Slot)
{
    verifyEnginef(VertexBuffer, "{} requires a non-null vertex buffer!", __FUNCTION__)
    verifyEnginef(m_BoundPipeline, "{} requires a graphics pipeline to be bound first!", __FUNCTION__)

    m_BoundVertexBuffer = Cast<COpenGLVertexBuffer>(VertexBuffer);

    glVertexArrayVertexBuffer(m_BoundPipeline->GetHandle(), Slot, m_BoundVertexBuffer->GetHandle(), 0,
        static_cast<int32>(m_BoundPipeline->GetStride()));
}

void COpenGLCommandList::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& IndexBuffer)
{
    verifyEnginef(IndexBuffer, "{} requires a non-null index buffer!", __FUNCTION__)
    verifyEnginef(m_BoundPipeline, "{} requires a graphics pipeline to be bound first!", __FUNCTION__)

    m_BoundIndexBuffer = Cast<COpenGLIndexBuffer>(IndexBuffer);

    glVertexArrayElementBuffer(m_BoundPipeline->GetHandle(), m_BoundIndexBuffer->GetHandle());
}

void COpenGLCommandList::SetTexture(const std::shared_ptr<ITexture>& Texture, uint32 Slot)
{
    verifyEnginef(Texture, "{} requires a non-null texture!", __FUNCTION__)

    Cast<COpenGLTexture>(Texture)->Bind(Slot);
}

// NOTE: (Ayydxn) The uniform buffer is already bound to its binding point in COpenGLUniformBuffer's constructor.
// This call is redundant and can technically be a no-op (atleast in OpenGL), but kept is intentionally to accurately reflect what's happening at draw time.
void COpenGLCommandList::SetUniformBuffer(const std::shared_ptr<IUniformBuffer>& UniformBuffer)
{
    verifyEnginef(UniformBuffer, "{} requires a non-null uniform buffer!", __FUNCTION__)
    
    Cast<COpenGLUniformBuffer>(UniformBuffer)->Bind();
}

void COpenGLCommandList::DrawIndexed(uint32 IndexCount, uint32 InstanceCount, uint32 FirstIndex)
{
    MOONLIGHT_PROFILE_GPU("DrawIndexed")

    verifyEnginef(m_BoundPipeline, "{} requires a bound graphics pipeline!!", __FUNCTION__)
    verifyEnginef(m_BoundVertexBuffer, "{} requires a bound vertex buffer!", __FUNCTION__)
    verifyEnginef(m_BoundIndexBuffer, "{} requires a bound index buffer!", __FUNCTION__)

    const int32 Count = IndexCount == 0 ? m_BoundIndexBuffer->GetCount() : IndexCount;
    
    glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, nullptr);
}
