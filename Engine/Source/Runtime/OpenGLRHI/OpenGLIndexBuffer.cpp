#include "MoonlightPCH.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

COpenGLIndexBuffer::COpenGLIndexBuffer(const void* Data, uint64 Size)
{
    glCreateBuffers(1, &m_IndexBufferHandle);
    glNamedBufferData(m_IndexBufferHandle, static_cast<int64>(Size) * sizeof(uint32), Data, GL_STATIC_DRAW);
}

COpenGLIndexBuffer::~COpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_IndexBufferHandle);
}
