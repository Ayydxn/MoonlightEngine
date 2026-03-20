#pragma once

#include "RHICore/Texture.h"

class COpenGLTexture : public ITexture
{
public:
    COpenGLTexture(const CTextureSpecification& Specification);
    COpenGLTexture(const std::filesystem::path& Filepath);
    ~COpenGLTexture() override;

    void Bind(uint32 Slot = 0) const override;
    
    void SetData(const void* Data, uint32 Size) override;
    
    uint32 GetWidth() const override { return m_Specification.Width; }
    uint32 GetHeight() const override { return m_Specification.Height; }
    uint64 GetNativeHandle() const override { return m_TextureHandle; }
    
    bool operator==(const ITexture& OtherTexture) const override
    {
        return m_TextureHandle == dynamic_cast<const COpenGLTexture&>(OtherTexture).m_TextureHandle;
    }
private:
    CTextureSpecification m_Specification;
    
    uint32 m_Width = 0;
    uint32 m_Height = 0;
    uint32 m_InternalFormat = 0;
    uint32 m_DataFormat = 0;
    uint32 m_DataType = 0;
    uint32 m_TextureHandle = 0;
};
