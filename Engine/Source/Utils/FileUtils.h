#pragma once

#include <filesystem>

class FFileUtils
{
public:
    static std::string ReadFile(const std::filesystem::path& Filepath);
};
