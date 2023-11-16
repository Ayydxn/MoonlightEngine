#include "MoonlightPCH.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

FOpenGLIndexBuffer::FOpenGLIndexBuffer(const void* BufferData, uint64 BufferSize)
{
    glCreateBuffers(1, &m_BufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int64>(BufferSize), BufferData, GL_STATIC_DRAW);
}

FOpenGLIndexBuffer::~FOpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_BufferHandle);
}

void FOpenGLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferHandle);
}
