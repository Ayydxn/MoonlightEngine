﻿#include "MoonlightPCH.h"
#include "Window.h"

#if defined(MOONLIGHT_PLATFORM_WIN64) || defined(MOONLIGHT_PLATFORM_MAC) || defined(MOONLIGHT_PLATFORM_LINUX)
    #include "DesktopWindow.h"
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

    #if defined(MOONLIGHT_PLATFORM_WIN64) || defined(MOONLIGHT_PLATFORM_MAC) || defined(MOONLIGHT_PLATFORM_LINUX)
        return std::make_shared<FDesktopWindow>();
    #else
        verifyEnginef(false, "Failed to create window '{}'! Unknown/unsupported platform!", Specification.Title)
        return nullptr;
    #endif
}
