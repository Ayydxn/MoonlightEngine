#include "MoonlightPCH.h"
#include "Input.h"
#include "CoreInputTypes.h"
#include "Application/Application.h"

#include <GLFW/glfw3.h>

void CInput::Initialize()
{
    CKeys::InitializeKeys();
}

bool CInput::GetKeyDown(const CKey& Key)
{
    return m_KeyData.contains(static_cast<int16>(Key.GetKeyCode())) && m_KeyData[static_cast<int32>(Key.GetKeyCode())].CurrentKeyState == EKeyState::Pressed;
}

bool CInput::GetKeyHeld(const CKey& Key)
{
    return m_KeyData.contains(static_cast<int16>(Key.GetKeyCode())) && m_KeyData[static_cast<int16>(Key.GetKeyCode())].CurrentKeyState == EKeyState::Held;
}

bool CInput::GetKeyUp(const CKey& Key)
{
    return m_KeyData.contains(static_cast<int16>(Key.GetKeyCode())) && m_KeyData[static_cast<int16>(Key.GetKeyCode())].CurrentKeyState == EKeyState::Released;
}

bool CInput::GetMouseButtonDown(const CKey& MouseButton)
{
    return m_MouseButtonData.contains(static_cast<int16>(MouseButton.GetKeyCode())) &&
        m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())].CurrentMouseButtonState == EKeyState::Pressed;
}

bool CInput::GetMouseButtonHeld(const CKey& MouseButton)
{
    return m_MouseButtonData.contains(static_cast<int16>(MouseButton.GetKeyCode())) &&
        m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())].CurrentMouseButtonState == EKeyState::Held;
}

bool CInput::GetMouseButtonUp(const CKey& MouseButton)
{
    return m_MouseButtonData.contains(static_cast<int16>(MouseButton.GetKeyCode())) &&
        m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())].CurrentMouseButtonState == EKeyState::Released;
}

float CInput::GetMouseX()
{
    auto[MouseX, MouseY] = GetMousePosition();
    return MouseX;
}

float CInput::GetMouseY()
{
    auto[MouseX, MouseY] = GetMousePosition();
    return MouseY;
}

std::pair<float, float> CInput::GetMousePosition()
{
    auto* Window = static_cast<GLFWwindow*>(CApplication::GetInstance().GetWindow().GetHandle());
    double MouseXPosition, MouseYPosition;

    glfwGetCursorPos(Window, &MouseXPosition, &MouseYPosition);

    return { static_cast<float>(MouseXPosition), static_cast<float>(MouseYPosition) };
}

void CInput::SetCursorMode(ECursorMode NewCursorMode)
{
    auto* Window = static_cast<GLFWwindow*>(CApplication::GetInstance().GetWindow().GetHandle());

    switch (NewCursorMode)
    {
        case ECursorMode::Normal:
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        }

        case ECursorMode::Hidden:
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }

        case ECursorMode::Locked:
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        }
    }

    m_CurrentCursorMode = NewCursorMode;
}

void CInput::TransitionPressedKeys()
{
    for (const auto& [Key, KeyData] : m_KeyData)
    {
        if (KeyData.CurrentKeyState == EKeyState::Pressed)
            UpdateKeyState(CKey::GetKeyFromKeyCode(static_cast<int16>(Key)), EKeyState::Held);
    }
}

void CInput::TransitionPressedMouseButtons()
{
    for (const auto& [MouseButton, MouseButtonData] : m_MouseButtonData)
    {
        if (MouseButtonData.CurrentMouseButtonState == EKeyState::Pressed)
            UpdateMouseButtonState(CKey::GetKeyFromKeyCode(static_cast<uint16>(MouseButton)), EKeyState::Held);
    }
}

void CInput::UpdateKeyState(const CKey& Key, EKeyState NewState)
{
    auto& KeyData = m_KeyData[static_cast<int16>(Key.GetKeyCode())];
    KeyData.Key = Key;
    KeyData.OldKeyState = KeyData.CurrentKeyState;
    KeyData.CurrentKeyState = NewState;
}

void CInput::UpdateMouseButtonState(const CKey& MouseButton, EKeyState NewState)
{
    auto& MouseButtonData = m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())];
    MouseButtonData.MouseButton = MouseButton;
    MouseButtonData.OldMouseButtonState = MouseButtonData.CurrentMouseButtonState;
    MouseButtonData.CurrentMouseButtonState = NewState;
}

void CInput::ClearReleasedKeys()
{
    for (const auto& [Key, KeyData] : m_KeyData)
    {
        if (KeyData.CurrentKeyState == EKeyState::Released)
            UpdateKeyState(CKey::GetKeyFromKeyCode(static_cast<uint16>(Key)), EKeyState::Unknown);
    }

    for (const auto& [MouseButton, MouseButtonData] : m_MouseButtonData)
    {
        if (MouseButtonData.CurrentMouseButtonState == EKeyState::Released)
            UpdateMouseButtonState(CKey::GetKeyFromKeyCode(static_cast<uint16>(MouseButton)), EKeyState::Unknown);
    }
}