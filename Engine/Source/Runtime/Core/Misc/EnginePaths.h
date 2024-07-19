#pragma once

#include <filesystem>

class CEnginePaths
{
public:
    static std::filesystem::path GetEngineDirectory();
    static std::filesystem::path GetCacheDirectory();
    static std::filesystem::path GetShaderCacheDirectory();
private:
    static void CreateDirectory(const std::filesystem::path& DirectoryPath);
};
