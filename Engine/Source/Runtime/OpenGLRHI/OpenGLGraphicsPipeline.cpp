#include "MoonlightPCH.h"
#include "OpenGLGraphicsPipeline.h"

#include <glad/glad.h>

FOpenGLGraphicsPipeline::FOpenGLGraphicsPipeline()
{
    verifyEnginef(m_Specification.VertexBufferLayout.GetElementCount() != 0, "FOpenGLGraphicsPipeline requires a non-empty vertex buffer layout!")

    FOpenGLGraphicsPipeline::Invalidate();
}

FOpenGLGraphicsPipeline::~FOpenGLGraphicsPipeline()
{
    glDeleteVertexArrays(1, &m_VertexArrayHandle);
}

void FOpenGLGraphicsPipeline::Invalidate()
{
    const FVertexBufferLayout VertexBufferLayout = m_Specification.VertexBufferLayout;
    uint32 AttributeIndex = 0;
    
    if (m_VertexArrayHandle != 0)
        glDeleteVertexArrays(1, &m_VertexArrayHandle);

    glGenVertexArrays(1, &m_VertexArrayHandle);
    glBindVertexArray(m_VertexArrayHandle);

    for (const auto& VertexBufferElement : VertexBufferLayout)
    {
        const uint32 ShaderDataTypeGLDataType = GetShaderDataTypeGLDataType(VertexBufferElement.ElementDataType);

        glEnableVertexAttribArray(AttributeIndex);

        if (ShaderDataTypeGLDataType == GL_INT)
        {
            glVertexAttribIPointer(AttributeIndex, static_cast<int32>(VertexBufferElement.GetComponentCount()), ShaderDataTypeGLDataType,
                static_cast<int32>(VertexBufferLayout.GetStride()), reinterpret_cast<const void*>(static_cast<intptr_t>(VertexBufferElement.Offset)));
        }
        else
        {
            glVertexAttribPointer(AttributeIndex, static_cast<int32>(VertexBufferElement.GetComponentCount()), ShaderDataTypeGLDataType,
                VertexBufferElement.bNormalized ? GL_TRUE : GL_FALSE, static_cast<int32>(VertexBufferLayout.GetStride()),
                reinterpret_cast<const void*>(static_cast<intptr_t>(VertexBufferElement.Offset)));
        }

        AttributeIndex++;
    }
}

void FOpenGLGraphicsPipeline::Bind()
{
    glBindVertexArray(m_VertexArrayHandle);
}

uint32 FOpenGLGraphicsPipeline::GetShaderDataTypeGLDataType(const EShaderDataType ShaderDataType)
{
    switch (ShaderDataType)
    {
        case EShaderDataType::Unknown: return 0;
        
        case EShaderDataType::Float:
        case EShaderDataType::Float2:
        case EShaderDataType::Float3:
        case EShaderDataType::Float4:
        case EShaderDataType::Matrix3:
        case EShaderDataType::Matrix4:
            return GL_FLOAT;
        
        case EShaderDataType::Int:
        case EShaderDataType::Int2:
        case EShaderDataType::Int3:
        case EShaderDataType::Int4:
            return GL_INT;
        
        case EShaderDataType::Boolean: return GL_BOOL;

        default:
        {
            assertEnginef(false, "Failed to get the OpenGL data type of the shader data type!")
            return 0;
        }
    }
}
