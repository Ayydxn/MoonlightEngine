#include "MoonlightPCH.h"
#include "EnginePaths.h"
#include "Utils/PlatformUtils.h"

std::filesystem::path FEnginePaths::GetEngineDirectory()
{
    std::filesystem::path EngineDirectory = FPlatformUtils::GetApplicationDirectory() / "Moonlight Engine";
    CreateDirectories(EngineDirectory);
    
    return EngineDirectory;
}

std::filesystem::path FEnginePaths::GetCacheDirectory()
{
    std::filesystem::path CacheDirectory =  GetEngineDirectory() / "Cache";
    CreateDirectories(CacheDirectory);
    
    return CacheDirectory;
}

std::filesystem::path FEnginePaths::GetShaderCacheDirectory()
{
    std::filesystem::path ShaderCacheDirectory = GetCacheDirectory() / "Shaders";
    CreateDirectories(ShaderCacheDirectory);
    
    return ShaderCacheDirectory;
}

void FEnginePaths::CreateDirectories(const std::filesystem::path& DirectoryPath)
{
    if (!std::filesystem::exists(DirectoryPath))
        std::filesystem::create_directories(DirectoryPath);
}
