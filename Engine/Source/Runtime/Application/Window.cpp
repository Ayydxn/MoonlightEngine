#include "MoonlightPCH.h"
#include "Window.h"

#ifdef MOONLIGHT_PLATFORM_WIN64
    #include "Windows/WindowsWindow.h"
#endif

std::shared_ptr<FWindow> FWindow::Create(const FWindowSpecification& Specification)
{
    m_Specification = Specification;

    m_WindowState.Title = Specification.Title;
    m_WindowState.Width = Specification.Width;
    m_WindowState.Height = Specification.Height;
    m_WindowState.bIsDecorationEnabled = Specification.bEnableDecoration;
    m_WindowState.WindowMode = Specification.WindowMode;
    m_WindowState.bIsVSyncEnabled = Specification.bEnableVSync;
    m_WindowState.bIsResizingEnabled = Specification.bEnableResizing;

    #ifdef MOONLIGHT_PLATFORM_WIN64
        return std::make_shared<FWindowsWindow>();
    #else
        verifyEnginef(false, "Failed to create window '{}'! Unknown/unsupported platform!", Specification.Title)
        return nullptr;
    #endif
}
