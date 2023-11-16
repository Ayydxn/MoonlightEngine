#include "MoonlightPCH.h"
#include "DesktopWindow.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "Input/CoreInputTypes.h"
#include "Input/Input.h"

#include <GLFW/glfw3.h>

namespace
{
    bool bIsGLFWInitialized = false; 
}

FDesktopWindow::~FDesktopWindow()
{
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();

    bIsGLFWInitialized = false;
}

void FDesktopWindow::Initialize()
{
    ENGINE_LOG_INFO_TAG("Core", "Creating window '{}' ({} x {})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

    if (!bIsGLFWInitialized)
    {
        verifyEnginef(glfwInit(), "Failed to create window '{}'! GLFW failed to initialize!", m_Specification.Title)

        glfwSetErrorCallback([](int32 ErrorCode, const char* ErrorMessage)
        {
            ENGINE_LOG_ERROR_TAG("GLFW", "GLFW Error: {} (Error Code: {})", ErrorMessage, ErrorCode);
        });

        bIsGLFWInitialized = true;
    }

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_DECORATED, m_Specification.bEnableDecoration);
    glfwWindowHint(GLFW_RESIZABLE, m_Specification.bEnableResizing);

    m_WindowHandle = glfwCreateWindow(static_cast<int32>(m_Specification.Width), static_cast<int32>(m_Specification.Height),
        m_Specification.Title.c_str(), nullptr, nullptr);
    
    verifyEnginef(m_WindowHandle, "Failed to create window '{}'!", m_Specification.Title)

    SetWindowMode(m_Specification.WindowMode);

    m_RendererContext = FRendererContext::Create(m_WindowHandle);
    m_RendererContext->Initialize();

    EnableVSync(m_Specification.bEnableVSync);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_WindowHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    else
        ENGINE_LOG_WARN_TAG("Core", "Raw mouse input is not available as it isn't supported on the current system");

    glfwSetWindowUserPointer(m_WindowHandle, &m_WindowState);

    glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* Window, int32 Key, int32 Scancode, int32 Action, int32 Modifiers)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        switch (Action)
        {
            case GLFW_PRESS:
            {
                const FKey PressedKey = FKey::GetKeyFromKeyCode(static_cast<uint16>(Key));
                FInput::UpdateKeyState(PressedKey, EKeyState::Pressed);

                FKeyPressedEvent KeyPressedEvent(PressedKey, false);
                WindowState.EventCallbackFunc(KeyPressedEvent);
                break;
            }

            case GLFW_RELEASE:
            {
                const FKey ReleasedKey = FKey::GetKeyFromKeyCode(static_cast<uint16>(Key));
                FInput::UpdateKeyState(ReleasedKey, EKeyState::Released);

                FKeyReleasedEvent KeyReleasedEvent(ReleasedKey);
                WindowState.EventCallbackFunc(KeyReleasedEvent);
                break;
            }

            case GLFW_REPEAT:
            {
                const FKey HeldDownKey = FKey::GetKeyFromKeyCode(static_cast<uint16>(Key));
                FInput::UpdateKeyState(HeldDownKey, EKeyState::Pressed);

                FKeyPressedEvent KeyPressedEvent(HeldDownKey, true);
                WindowState.EventCallbackFunc(KeyPressedEvent);
                break;
            }

            default:
                assertEnginef(false, "Unknown GLFW key action: {}", Action)
        }
    });

    glfwSetCharCallback(m_WindowHandle, [](GLFWwindow* Window, uint32 Codepoint)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FKeyTypedEvent KeyTypedEvent(Codepoint);
        WindowState.EventCallbackFunc(KeyTypedEvent);
    });

    glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* Window, int32 Button, int32 Action, int32 Modifiers)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        switch (Action)
        {
            case GLFW_PRESS:
            {
                const FKey PressedMouseButton = FKey::GetKeyFromKeyCode(static_cast<uint32>(Button));
                FInput::UpdateMouseButtonState(PressedMouseButton, EKeyState::Pressed);

                FMouseButtonPressedEvent MouseButtonPressedEvent(PressedMouseButton);
                WindowState.EventCallbackFunc(MouseButtonPressedEvent);
                break;
            }

            case GLFW_RELEASE:
            {
                const FKey ReleasedMouseButton = FKey::GetKeyFromKeyCode(static_cast<uint32>(Button));
                FInput::UpdateMouseButtonState(ReleasedMouseButton, EKeyState::Released);

                FMouseButtonReleasedEvent MouseButtonReleasedEvent(ReleasedMouseButton);
                WindowState.EventCallbackFunc(MouseButtonReleasedEvent);
                break;
            }

            default:
                assertEnginef(false, "Unknown GLFW mouse button action: {}", Action)
        }
    });

    glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* Window, double XPosition, double YPosition)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FMouseMovedEvent MouseMovedEvent(static_cast<float>(XPosition), static_cast<float>(YPosition));
        WindowState.EventCallbackFunc(MouseMovedEvent);
    });

    glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* Window, double XScroll, double YScroll)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FMouseScrolledEvent MouseScrolledEvent(static_cast<float>(XScroll), static_cast<float>(YScroll));
        WindowState.EventCallbackFunc(MouseScrolledEvent);
    });

    glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* Window)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FWindowCloseEvent WindowCloseEvent;
        WindowState.EventCallbackFunc(WindowCloseEvent);
    });

    glfwSetWindowIconifyCallback(m_WindowHandle, [](GLFWwindow* Window, int32 Minimized)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FWindowMinimizeEvent WindowMinimizeEvent(Minimized == GLFW_TRUE ? true : false);
        WindowState.EventCallbackFunc(WindowMinimizeEvent);
    });

    glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* Window, int32 Width, int32 Height)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FWindowResizeEvent WindowResizeEvent(Width, Height);
        WindowState.EventCallbackFunc(WindowResizeEvent);
    });

    glfwSetWindowPosCallback(m_WindowHandle, [](GLFWwindow* Window, int32 XPosition, int32 YPosition)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        FWindowMovedEvent WindowMovedEvent(XPosition, YPosition);
        WindowState.EventCallbackFunc(WindowMovedEvent);
    });

    glfwSetWindowFocusCallback(m_WindowHandle, [](GLFWwindow* Window, int32 Focused)
    {
        const auto& WindowState = *static_cast<FWindowState*>(glfwGetWindowUserPointer(Window));

        if (Focused == GLFW_TRUE)
        {
            FWindowFocusEvent WindowFocusEvent;
            WindowState.EventCallbackFunc(WindowFocusEvent);
        }
        else
        {
            FWindowLostFocusEvent WindowLostFocusEvent;
            WindowState.EventCallbackFunc(WindowLostFocusEvent);
        }
    });
}

void FDesktopWindow::ProcessEvents()
{
    glfwPollEvents();
}

void FDesktopWindow::SwapBuffers() const
{
    m_RendererContext->SwapBuffers();
}

std::pair<float, float> FDesktopWindow::GetPosition() const
{
    int32 XPosition, YPosition;
    glfwGetWindowPos(m_WindowHandle, &XPosition, &YPosition);

    return { static_cast<float>(XPosition), static_cast<float>(YPosition) };
}

void FDesktopWindow::Maximize() const
{
    glfwMaximizeWindow(m_WindowHandle);
}

void FDesktopWindow::CenterOnScreen() const
{
    const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int32 XPosition = VideoMode->width / 2 - static_cast<int32>(m_WindowState.Width) / 2; 
    const int32 YPosition = VideoMode->height / 2 - static_cast<int32>(m_WindowState.Height) / 2;

    glfwSetWindowPos(m_WindowHandle, XPosition, YPosition);
}

void FDesktopWindow::EnableVSync(bool bEnableVSync)
{
    m_RendererContext->SetVSync(bEnableVSync);

    m_WindowState.bIsVSyncEnabled = bEnableVSync;
}

void FDesktopWindow::SetWindowMode(EWindowMode NewWindowMode) const
{
    const GLFWvidmode* VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    switch (NewWindowMode)
    {
        case EWindowMode::Windowed:
        {
            glfwSetWindowMonitor(m_WindowHandle, nullptr, 100, 100, static_cast<int32>(m_Specification.Width),
                static_cast<int32>(m_Specification.Height), VideoMode->refreshRate);

            CenterOnScreen();

            break;
        }

        case EWindowMode::WindowedFullscreen:
        {
            glfwSetWindowMonitor(m_WindowHandle, glfwGetPrimaryMonitor(), 0, 0, VideoMode->width, VideoMode->height, VideoMode->refreshRate);

            break;
        }

        case EWindowMode::Fullscreen:
        {
            glfwSetWindowMonitor(m_WindowHandle, glfwGetPrimaryMonitor(), 0, 0, static_cast<int32>(m_Specification.Width),
                static_cast<int32>(m_Specification.Height), VideoMode->refreshRate);

            break;
        }
    }
    
    m_WindowState.WindowMode = NewWindowMode;
}

void FDesktopWindow::EnableResizing(bool bEnableResizing) const
{
    if (bEnableResizing && !m_WindowState.bIsResizingEnabled)
    {
        glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE, GLFW_TRUE);
        
        m_WindowState.bIsResizingEnabled = true;
    }
    else if (!bEnableResizing && m_WindowState.bIsResizingEnabled)
    {
        glfwSetWindowAttrib(m_WindowHandle, GLFW_RESIZABLE, GLFW_FALSE);
        
        m_WindowState.bIsResizingEnabled = false;
    }
}

void FDesktopWindow::SetTitle(const std::string& Title) const
{
    glfwSetWindowTitle(m_WindowHandle, Title.c_str());
    
    m_WindowState.Title = Title;
}
