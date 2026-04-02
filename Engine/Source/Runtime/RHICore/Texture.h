#pragma once

#include "CoreDefines.h"

#include <filesystem>
#include <memory>

enum class ETextureFormat : uint8
{
    None = 0,
    
    /* -- Color -- */
    RGBA8,
    RGBA16F,
    RGBA32F,
    R32I,
    
    /* -- Depth -- */
    Depth24Stencil8
};

struct CTextureSpecification
{
    uint32 Width = 1;
    uint32 Height = 1;
    ETextureFormat Format = ETextureFormat::RGBA8;
    bool bGenerateMips = false;
    bool bIsRenderTarget = false;
};

class MOONLIGHT_API ITexture
{
public:
    virtual ~ITexture() = default;
    
    virtual void Bind(uint32 Slot = 0) const = 0;

    virtual void SetData(const void* Data, uint32 Size) = 0;
    
    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;
    virtual uint64 GetNativeHandle() const = 0;
    
    virtual bool operator==(const ITexture& OtherTexture) const = 0;
};
