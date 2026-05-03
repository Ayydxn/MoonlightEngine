#include "MoonlightPCH.h"
#include "Window.h"

#if defined(MOONLIGHT_PLATFORM_WIN64) || defined(MOONLIGHT_PLATFORM_MAC) || defined(MOONLIGHT_PLATFORM_LINUX)
    #include "DesktopWindow.h"
#endif

std::shared_ptr<IWindow> IWindow::Create(const FWindowSpecification& Specification)
{
    #if defined(MOONLIGHT_PLATFORM_WIN64) || defined(MOONLIGHT_PLATFORM_MAC) || defined(MOONLIGHT_PLATFORM_LINUX)
        return std::make_shared<CDesktopWindow>(Specification);
    #else
        verifyEnginef(false, "Failed to create window '{}'! Unknown/unsupported platform!", Specification.Title)
        return nullptr;
    #endif
}
