#include "MoonlightPCH.h"
#include "Input.h"
#include "CoreInputTypes.h"
#include "Application/Application.h"

#include <GLFW/glfw3.h>

void FInput::Initialize()
{
    FKeys::InitializeKeys();
}

bool FInput::GetKeyDown(const FKey& Key)
{
    return m_KeyData.contains(static_cast<int16>(Key.GetKeyCode())) && m_KeyData[static_cast<int32>(Key.GetKeyCode())].CurrentKeyState == EKeyState::Pressed;
}

bool FInput::GetKeyHeld(const FKey& Key)
{
    return m_KeyData.contains(static_cast<int16>(Key.GetKeyCode())) && m_KeyData[static_cast<int16>(Key.GetKeyCode())].CurrentKeyState == EKeyState::Held;
}

bool FInput::GetKeyUp(const FKey& Key)
{
    return m_KeyData.contains(static_cast<int16>(Key.GetKeyCode())) && m_KeyData[static_cast<int16>(Key.GetKeyCode())].CurrentKeyState == EKeyState::Released;
}

bool FInput::GetMouseButtonDown(const FKey& MouseButton)
{
    return m_MouseButtonData.contains(static_cast<int16>(MouseButton.GetKeyCode())) &&
        m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())].CurrentMouseButtonState == EKeyState::Pressed;
}

bool FInput::GetMouseButtonHeld(const FKey& MouseButton)
{
    return m_MouseButtonData.contains(static_cast<int16>(MouseButton.GetKeyCode())) &&
        m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())].CurrentMouseButtonState == EKeyState::Held;
}

bool FInput::GetMouseButtonUp(const FKey& MouseButton)
{
    return m_MouseButtonData.contains(static_cast<int16>(MouseButton.GetKeyCode())) &&
        m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())].CurrentMouseButtonState == EKeyState::Released;
}

float FInput::GetMouseX()
{
    auto[MouseX, MouseY] = GetMousePosition();
    return MouseX;
}

float FInput::GetMouseY()
{
    auto[MouseX, MouseY] = GetMousePosition();
    return MouseY;
}

std::pair<float, float> FInput::GetMousePosition()
{
    auto* Window = static_cast<GLFWwindow*>(FApplication::GetInstance().GetWindow().GetHandle());
    double MouseXPosition, MouseYPosition;

    glfwGetCursorPos(Window, &MouseXPosition, &MouseYPosition);

    return { static_cast<float>(MouseXPosition), static_cast<float>(MouseYPosition) };
}

void FInput::SetCursorMode(ECursorMode NewCursorMode)
{
    auto* Window = static_cast<GLFWwindow*>(FApplication::GetInstance().GetWindow().GetHandle());

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

void FInput::TransitionPressedKeys()
{
    for (const auto& [Key, KeyData] : m_KeyData)
    {
        if (KeyData.CurrentKeyState == EKeyState::Pressed)
            UpdateKeyState(FKey::GetKeyFromKeyCode(static_cast<int16>(Key)), EKeyState::Held);
    }
}

void FInput::TransitionPressedMouseButtons()
{
    for (const auto& [MouseButton, MouseButtonData] : m_MouseButtonData)
    {
        if (MouseButtonData.CurrentMouseButtonState == EKeyState::Pressed)
            UpdateMouseButtonState(FKey::GetKeyFromKeyCode(static_cast<uint16>(MouseButton)), EKeyState::Held);
    }
}

void FInput::UpdateKeyState(const FKey& Key, EKeyState NewState)
{
    auto& KeyData = m_KeyData[static_cast<int16>(Key.GetKeyCode())];
    KeyData.Key = Key;
    KeyData.OldKeyState = KeyData.CurrentKeyState;
    KeyData.CurrentKeyState = NewState;
}

void FInput::UpdateMouseButtonState(const FKey& MouseButton, EKeyState NewState)
{
    auto& MouseButtonData = m_MouseButtonData[static_cast<int16>(MouseButton.GetKeyCode())];
    MouseButtonData.MouseButton = MouseButton;
    MouseButtonData.OldMouseButtonState = MouseButtonData.CurrentMouseButtonState;
    MouseButtonData.CurrentMouseButtonState = NewState;
}

void FInput::ClearReleasedKeys()
{
    for (const auto& [Key, KeyData] : m_KeyData)
    {
        if (KeyData.CurrentKeyState == EKeyState::Released)
            UpdateKeyState(FKey::GetKeyFromKeyCode(static_cast<uint16>(Key)), EKeyState::Unknown);
    }

    for (const auto& [MouseButton, MouseButtonData] : m_MouseButtonData)
    {
        if (MouseButtonData.CurrentMouseButtonState == EKeyState::Released)
            UpdateMouseButtonState(FKey::GetKeyFromKeyCode(static_cast<uint16>(MouseButton)), EKeyState::Unknown);
    }
}