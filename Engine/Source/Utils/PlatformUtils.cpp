#include "MoonlightPCH.h"
#include "PlatformUtils.h"
#include "Windows/WindowsPlatformUtils.h"
#include "Linux/LinuxPlatformUtils.h"

std::shared_ptr<IPlatformUtils> IPlatformUtils::CreatePlatformUtilsInstance()
{
    #ifdef MOONLIGHT_PLATFORM_WIN64
        return std::make_shared<CWindowsPlatformUtils>();
    #elif defined(MOONLIGHT_PLATFORM_LINUX)
        return std::make_shared<CLinuxPlatformUtils>();
    #else
        verifyEnginef(false, "Failed to create platform utils! Unknown/unsupported platform!")
        return nullptr;
    #endif
}

std::filesystem::path IPlatformUtils::GetApplicationDirectory()
{
    return m_PlatformUtilsInstance->GetApplicationDirectory_Implementation();
}
