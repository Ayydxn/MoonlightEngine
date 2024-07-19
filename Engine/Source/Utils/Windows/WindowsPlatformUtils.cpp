#include "MoonlightPCH.h"
#include "WindowsPlatformUtils.h"

#include <ShlObj_core.h>

std::filesystem::path CWindowsPlatformUtils::GetApplicationDirectory_Implementation()
{
    WCHAR* LocalAppDataDirectory = nullptr;
    verifyEnginef(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, nullptr, &LocalAppDataDirectory)), "Failed to get local app data directory!")

    const auto LocalAppDataDirectoryWideString = std::wstring(LocalAppDataDirectory);
    
    return std::string(LocalAppDataDirectoryWideString.begin(), LocalAppDataDirectoryWideString.end());
}
