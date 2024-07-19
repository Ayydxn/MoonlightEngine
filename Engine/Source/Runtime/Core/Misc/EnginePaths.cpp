#include "MoonlightPCH.h"
#include "EnginePaths.h"
#include "Utils/PlatformUtils.h"

std::filesystem::path CEnginePaths::GetEngineDirectory()
{
    std::filesystem::path EngineDirectory = IPlatformUtils::GetApplicationDirectory() / "Moonlight Engine";
    CreateDirectory(EngineDirectory);
    
    return EngineDirectory;
}

std::filesystem::path CEnginePaths::GetCacheDirectory()
{
    std::filesystem::path CacheDirectory =  GetEngineDirectory() / "Cache";
    CreateDirectory(CacheDirectory);
    
    return CacheDirectory;
}

std::filesystem::path CEnginePaths::GetShaderCacheDirectory()
{
    std::filesystem::path ShaderCacheDirectory = GetCacheDirectory() / "Shaders";
    CreateDirectory(ShaderCacheDirectory);
    
    return ShaderCacheDirectory;
}

void CEnginePaths::CreateDirectory(const std::filesystem::path& DirectoryPath)
{
    if (!std::filesystem::exists(DirectoryPath))
        std::filesystem::create_directories(DirectoryPath);
}
