#include "MoonlightPCH.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace
{
    GLenum ConvertMoonlightVertexBufferUsageToOpenGL(EVertexBufferUsage VertexBufferUsage)
    {
        switch (VertexBufferUsage)
        {
            case EVertexBufferUsage::Static: return GL_STATIC_DRAW;
            case EVertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        }

        return -1;
    }
}

COpenGLVertexBuffer::COpenGLVertexBuffer(const void* Data, uint64 Size, EVertexBufferUsage Usage)
{
    glCreateBuffers(1, &m_VertexBufferHandle);
    glNamedBufferData(m_VertexBufferHandle, static_cast<int64>(Size), Data, ConvertMoonlightVertexBufferUsageToOpenGL(Usage));
}

COpenGLVertexBuffer::COpenGLVertexBuffer(uint64 Size, EVertexBufferUsage Usage)
{
    glCreateBuffers(1, &m_VertexBufferHandle);
    glNamedBufferData(m_VertexBufferHandle, static_cast<int64>(Size), nullptr, ConvertMoonlightVertexBufferUsageToOpenGL(Usage));
}

COpenGLVertexBuffer::~COpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_VertexBufferHandle);
}
