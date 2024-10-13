#include "MoonlightPCH.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

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

    verifyEnginef(InternalImageFormat & ImageDataFormat, "Format of the texture '{}' is not supported!", Filepath.string())

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);

    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void COpenGLTexture::Bind(uint32 Slot) const
{
    glBindTextureUnit(Slot, m_TextureHandle);
}
