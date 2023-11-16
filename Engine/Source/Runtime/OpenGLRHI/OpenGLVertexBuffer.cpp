#include "MoonlightPCH.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

FOpenGLVertexBuffer::FOpenGLVertexBuffer(const void* BufferData, uint64 BufferSize)
{
    glCreateBuffers(1, &m_BufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int64>(BufferSize), BufferData, GL_STATIC_DRAW);
    
}

FOpenGLVertexBuffer::~FOpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_BufferHandle);
}

void FOpenGLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferHandle);
}
