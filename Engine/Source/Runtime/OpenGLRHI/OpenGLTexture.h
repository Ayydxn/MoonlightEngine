#pragma once

#include "RHICore/Texture.h"

class COpenGLTexture : public ITexture
{
public:
    COpenGLTexture(uint32 Width, uint32 Height);
    COpenGLTexture(const std::filesystem::path& Filepath);
    ~COpenGLTexture() override;

    void SetData(const void* Data, uint32 Size) override;

    void Bind(uint32 Slot = 0) const override;
    
    bool operator==(const ITexture& OtherTexture) const override
    {
        return m_TextureHandle == dynamic_cast<const COpenGLTexture&>(OtherTexture).m_TextureHandle;
    }
private:
    uint32 m_Width = 0;
    uint32 m_Height = 0;
    uint32 m_InternalFormat = 0;
    uint32 m_DataFormat = 0;
    uint32 m_TextureHandle = 0;
};
