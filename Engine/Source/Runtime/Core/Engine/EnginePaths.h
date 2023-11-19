#pragma once

#include <filesystem>

class FEnginePaths
{
public:
    static std::filesystem::path GetEngineDirectory();
    static std::filesystem::path GetCacheDirectory();
    static std::filesystem::path GetShaderCacheDirectory();
private:
    static void CreateDirectories(const std::filesystem::path& DirectoryPath);
};
