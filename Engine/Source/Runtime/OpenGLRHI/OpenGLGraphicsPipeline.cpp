#include "MoonlightPCH.h"
#include "OpenGLGraphicsPipeline.h"

#include <glad/glad.h>

namespace
{
    GLenum GetShaderDataTypeOpenGLBaseType(EShaderDataType ShaderDataType)
    {
        switch (ShaderDataType)
        {
            case EShaderDataType::Float:
            case EShaderDataType::Float2:
            case EShaderDataType::Float3:
            case EShaderDataType::Float4:
            case EShaderDataType::Matrix3x3:
            case EShaderDataType::Matrix4x4:
                return GL_FLOAT;
            
            case EShaderDataType::Int:
            case EShaderDataType::Int2:
            case EShaderDataType::Int3:
            case EShaderDataType::Int4:
                return GL_INT;
            
            case EShaderDataType::Boolean: return GL_BOOL;
        }

        verifyEnginef(false, "Failed to get OpenGL base type of shader data type!");
        return 0;
    }
}

COpenGLGraphicsPipeline::COpenGLGraphicsPipeline()
{
    verifyEnginef(m_Specification.VertexBufferLayout.GetElementCount() != 0, "An OpenGL graphics pipeline requires a non-empty vertex buffer layout!")

    COpenGLGraphicsPipeline::Invalidate();
}

COpenGLGraphicsPipeline::~COpenGLGraphicsPipeline()
{
    glDeleteVertexArrays(1, &m_VertexArrayHandle);
}

void COpenGLGraphicsPipeline::Invalidate()
{
    uint32 AttributeIndex = 0;
    
    glCreateVertexArrays(1, &m_VertexArrayHandle);

    for (const auto& VertexBufferElement : m_Specification.VertexBufferLayout)
    {
        const uint32 ShaderDataTypeOpenGLBaseType = GetShaderDataTypeOpenGLBaseType(VertexBufferElement.DataType);
        
        glEnableVertexArrayAttrib(m_VertexArrayHandle, AttributeIndex);

        if (ShaderDataTypeOpenGLBaseType == GL_INT)
        {
            glVertexArrayAttribIFormat(m_VertexArrayHandle, AttributeIndex, static_cast<int32>(VertexBufferElement.GetComponentCount()),
                ShaderDataTypeOpenGLBaseType, VertexBufferElement.Offset);
        }
        else
        {
            glVertexArrayAttribFormat(m_VertexArrayHandle, AttributeIndex, static_cast<int32>(VertexBufferElement.GetComponentCount()),
                ShaderDataTypeOpenGLBaseType, VertexBufferElement.bIsNormalized, VertexBufferElement.Offset);
        }

        glVertexArrayAttribBinding(m_VertexArrayHandle, AttributeIndex, 0);
        
        AttributeIndex++;
    }
}
