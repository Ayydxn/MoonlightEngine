#include "MoonlightPCH.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

COpenGLUniformBuffer::COpenGLUniformBuffer(uint32 Size, uint32_t Binding)
{
    glCreateBuffers(1, &m_UniformBufferHandle);
    glNamedBufferData(m_UniformBufferHandle, Size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, Binding, m_UniformBufferHandle);
}

COpenGLUniformBuffer::~COpenGLUniformBuffer()
{
    glDeleteBuffers(1, &m_UniformBufferHandle);
}

void COpenGLUniformBuffer::SetData(const void* Data, uint32_t Size, uint32_t Offset)
{
    glNamedBufferSubData(m_UniformBufferHandle, Offset, Size, Data);
}
