#include "MoonlightPCH.h"
#include "PlatformUtils.h"
#include "Windows/WindowsPlatformUtils.h"

std::filesystem::path FPlatformUtils::GetApplicationDirectory()
{
    return m_PlatformUtilsInstance->GetApplicationDirectory_Implementation();
}

std::shared_ptr<FPlatformUtils> FPlatformUtils::CreatePlatformUtilsInstance()
{
    #ifdef MOONLIGHT_PLATFORM_WIN64
        return std::make_shared<FWindowsPlatformUtils>();
    #else
        verifyEnginef(false, "Failed to create platform utils! Unknown/unsupported platform!")
        return nullptr;
    #endif
}
