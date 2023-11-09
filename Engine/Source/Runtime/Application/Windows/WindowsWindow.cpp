#include "MoonlightPCH.h"
#include "WindowsWindow.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

void FWindowsWindow::Initialize()
{
    ENGINE_LOG_INFO_TAG("Core", "Creating window '{}' ({} x {})", m_Specification.Title, m_Specification.Width, m_Specification.Height);
    
    const HINSTANCE HInstance = GetModuleHandleW(nullptr);
    const auto WindowClassName = L"Moonlight Engine Window Class";

    WNDCLASSW WindowClass = {};
    WindowClass.style = CS_DBLCLKS;
    WindowClass.lpfnWndProc = FWindowsWindow::WindowProcedure;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = HInstance;
    WindowClass.hIcon = LoadIconW(HInstance, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorW(HInstance, IDC_ARROW);
    WindowClass.hbrBackground = nullptr;
    WindowClass.lpszClassName = WindowClassName;

    verifyEnginef(RegisterClassW(&WindowClass), "Failed to register Win32 window class for window '{}'!", m_WindowState.Title)

    uint32 WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    constexpr uint32 WindowExStyle = WS_EX_APPWINDOW;

    if (m_Specification.bEnableResizing)
        WindowStyle |= WS_THICKFRAME;

    RECT WindowRect = { 0, 0, 0, 0 };
    AdjustWindowRectEx(&WindowRect, WindowStyle, false, WindowExStyle);
    
    uint32 WindowWidth = m_Specification.Width;
    uint32 WindowHeight = m_Specification.Height;
    WindowWidth += WindowRect.right - WindowRect.left;
    WindowHeight += WindowRect.bottom - WindowRect.top;
    
    m_WindowHandle = CreateWindowExW(WindowExStyle, WindowClassName, std::wstring(m_WindowState.Title.begin(), m_WindowState.Title.end()).c_str(),
        WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int32>(WindowWidth), static_cast<int32>(WindowHeight),
        nullptr, nullptr, HInstance, nullptr);
    
    verifyEnginef(m_WindowHandle, "Failed to create window '{}'!", m_WindowState.Title)

    ShowWindow(m_WindowHandle, SW_SHOW);
    
    SetWindowLongPtrW(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_WindowState));
    
    CenterOnScreen();
}

void FWindowsWindow::ProcessEvents()
{
    MSG Message;
    while (PeekMessageW(&Message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }
}

void FWindowsWindow::SwapBuffers() const
{
    // TODO: (Ayydan) Implement.
}

std::pair<float, float> FWindowsWindow::GetPosition() const
{
    POINT WindowPosition = { 0, 0 };
    ClientToScreen(m_WindowHandle, &WindowPosition);

    return { static_cast<float>(WindowPosition.x), static_cast<float>(WindowPosition.y) };
}

void FWindowsWindow::Maximize() const
{
    ShowWindow(m_WindowHandle, SW_MAXIMIZE);
}

void FWindowsWindow::CenterOnScreen() const
{
    const uint32 WindowXPosition = GetSystemMetrics(SM_CXSCREEN) / 2 - m_WindowState.Width / 2;
    const uint32 WindowYPosition = GetSystemMetrics(SM_CYSCREEN) / 2 - m_WindowState.Height / 2;

    SetWindowPos(m_WindowHandle, HWND_TOP, static_cast<int32>(WindowXPosition), static_cast<int32>(WindowYPosition),
        static_cast<int32>(m_WindowState.Width), static_cast<int32>(m_WindowState.Height), SWP_SHOWWINDOW);
}

void FWindowsWindow::EnableVSync(bool bEnableVSync)
{
    // TODO: (Ayydan) Implement.

    m_WindowState.bIsVSyncEnabled = bEnableVSync;
}

void FWindowsWindow::SetWindowMode(EWindowMode NewWindowMode) const
{
    // TODO: (Ayydan) Implement.
    switch (m_Specification.WindowMode)
    {
        case EWindowMode::Windowed:
        {
            break;
        }

        case EWindowMode::WindowedFullscreen:
        {
            break;
        }

        case EWindowMode::Fullscreen:
        {
            break;
        }
    }
    
    m_WindowState.WindowMode = NewWindowMode;
}

void FWindowsWindow::EnableResizing(bool bEnableResizing) const
{
    if (bEnableResizing && !m_WindowState.bIsResizingEnabled)
    {
        int32 WindowStyle = GetWindowLongW(m_WindowHandle, GWL_STYLE);
        WindowStyle |= WS_THICKFRAME;

        SetWindowLongW(m_WindowHandle, GWL_STYLE, WindowStyle);

        m_WindowState.bIsResizingEnabled = true;
    }
    else if (!bEnableResizing && m_WindowState.bIsResizingEnabled)
    {
        int32 WindowStyle = GetWindowLongW(m_WindowHandle, GWL_STYLE);
        WindowStyle &= ~WS_THICKFRAME;

        SetWindowLongW(m_WindowHandle, GWL_STYLE, WindowStyle);

        m_WindowState.bIsResizingEnabled = false;
    }
}

void FWindowsWindow::SetTitle(const std::string& Title) const
{
    SetWindowTextW(m_WindowHandle, std::wstring(Title.begin(), Title.end()).c_str());

    m_WindowState.Title = Title;
}

LRESULT FWindowsWindow::WindowProcedure(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    const auto WindowState = reinterpret_cast<FWindowState*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
    if (WindowState == nullptr || WindowState->EventCallbackFunc == nullptr)
        return DefWindowProcW(Window, Message, WParam, LParam);
    
    switch (Message)
    {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            const bool bPreviousKeyStateFlag = (HIWORD(LParam) & KF_REPEAT) == KF_REPEAT;
            const int32 RepeatCount = LOWORD(LParam);

            // The second parameter evaluates to true if the previous key state and the current one is repeat.
            FKeyPressedEvent KeyPressedEvent(static_cast<uint32>(WParam), bPreviousKeyStateFlag == 1 && RepeatCount > 0);
            WindowState->EventCallbackFunc(KeyPressedEvent);
            break;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            FKeyReleasedEvent KeyReleasedEvent(static_cast<uint32>(WParam));
            WindowState->EventCallbackFunc(KeyReleasedEvent);

            break;
        }

        case WM_CHAR:
        case WM_SYSCHAR:
        {
            FKeyTypedEvent KeyTypedEvent(static_cast<uint32>(WParam));
            WindowState->EventCallbackFunc(KeyTypedEvent);
            
            break;
        }

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        {
            uint32 MouseButton;

            if (Message == WM_LBUTTONDOWN)
            {
                MouseButton = VK_LBUTTON;
            }
            else if (Message == WM_RBUTTONDOWN)
            {
                MouseButton = VK_RBUTTON;
            }
            else if (Message == WM_MBUTTONDOWN)
            {
                MouseButton = VK_MBUTTON;
            }
            else if (GET_XBUTTON_WPARAM(WParam) == XBUTTON2)
            {
                MouseButton = VK_XBUTTON2;
            }
            else
            {
                MouseButton = VK_XBUTTON1;
            }

            FMouseButtonPressedEvent MouseButtonPressedEvent(MouseButton);
            WindowState->EventCallbackFunc(MouseButtonPressedEvent);
            
            break;
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            uint32 MouseButton;

            if (Message == WM_LBUTTONUP)
            {
                MouseButton = VK_LBUTTON;
            }
            else if (Message == WM_RBUTTONUP)
            {
                MouseButton = VK_RBUTTON;
            }
            else if (Message == WM_MBUTTONUP)
            {
                MouseButton = VK_MBUTTON;
            }
            else if (GET_XBUTTON_WPARAM(WParam) == XBUTTON2)
            {
                MouseButton = VK_XBUTTON2;
            }
            else
            {
                MouseButton = VK_XBUTTON1;
            }

            FMouseButtonReleasedEvent MouseButtonReleasedEvent(MouseButton);
            WindowState->EventCallbackFunc(MouseButtonReleasedEvent);
            
            break;
        }

        case WM_MOUSEMOVE:
        {
            const float XPosition = static_cast<float>(GET_X_LPARAM(LParam));
            const float YPosition = static_cast<float>(GET_Y_LPARAM(LParam));

            FMouseMovedEvent MouseMovedEvent(XPosition, YPosition);
            WindowState->EventCallbackFunc(MouseMovedEvent);

            break;
        }

        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
        {
            const double XScrollOffset = -(static_cast<SHORT>(HIWORD(WParam)) / static_cast<double>(WHEEL_DELTA));
            const double YScrollOffset = static_cast<SHORT>(HIWORD(WParam)) / static_cast<double>(WHEEL_DELTA);

            FMouseScrolledEvent MouseScrolledEvent(static_cast<float>(XScrollOffset), static_cast<float>(YScrollOffset));
            WindowState->EventCallbackFunc(MouseScrolledEvent);

            break;
        }
        
        case WM_CLOSE:
        {
            DestroyWindow(Window);
            
            FWindowCloseEvent WindowCloseEvent;
            WindowState->EventCallbackFunc(WindowCloseEvent);

            break;
        }

        case WM_SYSCOMMAND:
        {
            if (WParam == SC_MINIMIZE)
            {
                FWindowMinimizeEvent WindowMinimizeEvent(true);
                WindowState->EventCallbackFunc(WindowMinimizeEvent);
            }
            else if (WParam == SC_RESTORE)
            {
                FWindowMinimizeEvent WindowMinimizeEvent(false);
                WindowState->EventCallbackFunc(WindowMinimizeEvent);
            }

            break;
        }

        case WM_SIZE:
        {
            const uint32 NewWindowWidth = LOWORD(LParam);
            const uint32 NewWindowHeight = HIWORD(LParam);

            FWindowResizeEvent WindowResizeEvent(NewWindowWidth, NewWindowHeight);
            WindowState->EventCallbackFunc(WindowResizeEvent);
            
            break;
        }

        case WM_MOVE:
        {
            const int32 XPosition = static_cast<short>(LOWORD(LParam));   // horizontal position 
            const int32 YPosition = static_cast<short>(HIWORD(LParam));   // vertical position

            FWindowMovedEvent WindowMovedEvent(XPosition, YPosition);
            WindowState->EventCallbackFunc(WindowMovedEvent);
            
            break;
        }

        case WM_SETFOCUS:
        {
            FWindowFocusEvent WindowFocusEvent;
            WindowState->EventCallbackFunc(WindowFocusEvent);

            break;
        }

        case WM_KILLFOCUS:
        {
            FWindowLostFocusEvent WindowLostFocusEvent;
            WindowState->EventCallbackFunc(WindowLostFocusEvent);

            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(EXIT_SUCCESS);
            return EXIT_SUCCESS;
        }

        case WM_ERASEBKGND:
        {
            // Tell Windows that erasing the window's background will be handled by the application.
            return 1;
        }

        default:
            return DefWindowProcW(Window, Message, WParam, LParam);
    }

    return DefWindowProcW(Window, Message, WParam, LParam);
}
