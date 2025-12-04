#include "MoonlightPCH.h"
#include "LinuxPlatformUtils.h"

#include <cstdlib>

std::filesystem::path CLinuxPlatformUtils::GetApplicationDirectory_Implementation()
{
    if (const char* homeDir = std::getenv("HOME"))
        return std::filesystem::path(homeDir);

    return std::filesystem::current_path();
}
