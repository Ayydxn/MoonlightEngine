#pragma once

#include "RHICore/Texture.h"

class COpenGLTexture : public ITexture
{
public:
    COpenGLTexture(const std::filesystem::path& Filepath);
    ~COpenGLTexture() override;

    void Bind(uint32 Slot = 0) const;
private:
    uint32 m_TextureHandle = 0;
};
