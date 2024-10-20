#pragma once

#include "CoreDefines.h"

#include <filesystem>
#include <memory>

class MOONLIGHT_API ITexture
{
public:
    virtual ~ITexture() = default;

    static std::shared_ptr<ITexture> Create(const std::filesystem::path& Filepath);
};
