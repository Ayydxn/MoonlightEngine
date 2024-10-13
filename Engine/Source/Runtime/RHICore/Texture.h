#pragma once

#include <filesystem>
#include <memory>

class ITexture
{
public:
    virtual ~ITexture() = default;

    static std::shared_ptr<ITexture> Create(const std::filesystem::path& Filepath);
};
