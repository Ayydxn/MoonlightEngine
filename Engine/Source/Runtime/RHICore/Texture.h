#pragma once

#include "CoreDefines.h"

#include <filesystem>
#include <memory>

class MOONLIGHT_API ITexture
{
public:
    virtual ~ITexture() = default;
    
    virtual void Bind(uint32 Slot = 0) const = 0;

    virtual void SetData(const void* Data, uint32 Size) = 0;
    
    static std::shared_ptr<ITexture> Create(uint32 Width, uint32 Height);
    static std::shared_ptr<ITexture> Create(const std::filesystem::path& Filepath);
    
    virtual bool operator==(const ITexture& OtherTexture) const = 0;
};
