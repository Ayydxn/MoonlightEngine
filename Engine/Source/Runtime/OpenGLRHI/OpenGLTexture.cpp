#include "MoonlightPCH.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

COpenGLTexture::COpenGLTexture(uint32 Width, uint32 Height)
    : m_Width(Width), m_Height(Height)
{
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);

    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(m_TextureHandle, 1, m_InternalFormat, static_cast<int32>(Width), static_cast<int32>(Height));
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

    verifyEnginef(InternalImageFormat & ImageDataFormat, "Format of the texture '{}' is not supported!", Filepath.string())

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);

    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(m_TextureHandle, 1, InternalImageFormat, Width, Height);
    glTextureSubImage2D(m_TextureHandle, 0, 0, 0, Width, Height, ImageDataFormat, GL_UNSIGNED_BYTE, PixelData);

    stbi_image_free(PixelData);
}

COpenGLTexture::~COpenGLTexture()
{
    glDeleteTextures(1, &m_TextureHandle);
}

void COpenGLTexture::SetData(const void* Data, uint32 Size)
{
    const uint32 BytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
    assertEngine(Size == m_Width * m_Height * BytesPerPixel)
    
    glTextureSubImage2D(m_TextureHandle, 0, 0, 0, static_cast<int32>(m_Width), static_cast<int32>(m_Height), m_DataFormat, GL_UNSIGNED_BYTE, Data);
}

void COpenGLTexture::Bind(uint32 Slot) const
{
    glBindTextureUnit(Slot, m_TextureHandle);
}
