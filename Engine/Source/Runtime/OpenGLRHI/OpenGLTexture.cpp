#include "MoonlightPCH.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace
{
    void DetermineGLFormats(ETextureFormat InputFormat, uint32& OutInternalFormat, uint32& OutDataFormat, uint32& OutDataType)
    {
        switch (InputFormat)
        {
            case ETextureFormat::RGBA8:
            {
                OutInternalFormat = GL_RGBA8;
                OutDataFormat = GL_RGBA;
                OutDataType = GL_UNSIGNED_BYTE;
                break;
            }
            
            case ETextureFormat::RGBA16F:
            {
                OutInternalFormat = GL_RGBA16F;
                OutDataFormat = GL_RGBA;
                OutDataType = GL_FLOAT;
                break;
            }
            
            case ETextureFormat::RGBA32F:
            {
                OutInternalFormat = GL_RGBA32F;
                OutDataFormat = GL_RGBA;
                OutDataType = GL_FLOAT;
                break;
            }
            
            case ETextureFormat::R32I:
            {
                OutInternalFormat = GL_R32I;
                OutDataFormat = GL_RED_INTEGER;
                OutDataType = GL_INT;
                break;
            }
            
            case ETextureFormat::Depth24Stencil8:
            {
                OutInternalFormat = GL_DEPTH24_STENCIL8;
                OutDataFormat = GL_DEPTH_STENCIL;
                OutDataType = GL_UNSIGNED_INT_24_8;
                break;
            }
            
            default: verifyEnginef(false, "Failed to determine OpenGL formats for unknown Moonlight texture format!") break;
        }
    }
}

COpenGLTexture::COpenGLTexture(const CTextureSpecification& Specification)
    : m_Specification(Specification), m_Width(Specification.Width), m_Height(Specification.Height)
{
    DetermineGLFormats(Specification.Format, m_InternalFormat, m_DataFormat, m_DataType);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);

    if (m_Specification.bIsRenderTarget)
    {
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
    {
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glTextureStorage2D(m_TextureHandle, 1, m_InternalFormat, static_cast<int32>(Specification.Width), static_cast<int32>(Specification.Height));
}

COpenGLTexture::COpenGLTexture(const std::filesystem::path& Filepath)
{
    stbi_set_flip_vertically_on_load(true);
    
    int32 Width, Height, Channels;
    uint32 InternalImageFormat = 0, ImageDataFormat = 0;
    
    stbi_uc* PixelData = stbi_load(Filepath.string().c_str(), &Width, &Height, &Channels, 0);
    verifyEnginef(PixelData, "Failed to load texture '{}'!", Filepath.string())
   
    if (Channels == 4)
    {
        InternalImageFormat = GL_RGBA8;
        ImageDataFormat = GL_RGBA;
    }
    else if (Channels == 3)
    {
        InternalImageFormat = GL_RGB8;
        ImageDataFormat = GL_RGB;
    }

    m_Width = Width;
    m_Height = Height;
    m_InternalFormat = InternalImageFormat;
    m_DataFormat = ImageDataFormat;
    m_DataType = GL_UNSIGNED_BYTE;
    
    verifyEnginef(InternalImageFormat & ImageDataFormat, "Format of the texture '{}' is not supported!", Filepath.string())

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);

    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(m_TextureHandle, 1, InternalImageFormat, Width, Height);
    glTextureSubImage2D(m_TextureHandle, 0, 0, 0, Width, Height, ImageDataFormat, m_DataType, PixelData);

    stbi_image_free(PixelData);
}

COpenGLTexture::~COpenGLTexture()
{
    glDeleteTextures(1, &m_TextureHandle);
}

void COpenGLTexture::Bind(uint32 Slot) const
{
    glBindTextureUnit(Slot, m_TextureHandle);
}

void COpenGLTexture::SetData(const void* Data, uint32 Size)
{
    const uint32 BytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
    assertEngine(Size == m_Width * m_Height * BytesPerPixel)
    
    glTextureSubImage2D(m_TextureHandle, 0, 0, 0, static_cast<int32>(m_Width), static_cast<int32>(m_Height), m_DataFormat, m_DataType, Data);
}
