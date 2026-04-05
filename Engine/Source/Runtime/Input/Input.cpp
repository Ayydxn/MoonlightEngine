#include "MoonlightPCH.h"
#include "Input.h"
#include "Application/Application.h"

#include <GLFW/glfw3.h>
#include <cmath>

void CKeys::InitializeKeys()
{
    /* Printable */
    AddKeyToMap(Space);
    AddKeyToMap(Apostrophe);
    AddKeyToMap(Comma);
    AddKeyToMap(Minus);
    AddKeyToMap(Period);
    AddKeyToMap(Slash);
    AddKeyToMap(Zero);
    AddKeyToMap(One);
    AddKeyToMap(Two);
    AddKeyToMap(Three);
    AddKeyToMap(Four);
    AddKeyToMap(Five);
    AddKeyToMap(Six);
    AddKeyToMap(Seven);
    AddKeyToMap(Eight);
    AddKeyToMap(Nine);
    AddKeyToMap(Semicolon);
    AddKeyToMap(Equal);
    AddKeyToMap(A);
    AddKeyToMap(B);
    AddKeyToMap(C);
    AddKeyToMap(D);
    AddKeyToMap(E);
    AddKeyToMap(F);
    AddKeyToMap(G);
    AddKeyToMap(H);
    AddKeyToMap(I);
    AddKeyToMap(J);
    AddKeyToMap(K);
    AddKeyToMap(L);
    AddKeyToMap(M);
    AddKeyToMap(N);
    AddKeyToMap(O);
    AddKeyToMap(P);
    AddKeyToMap(Q);
    AddKeyToMap(R);
    AddKeyToMap(S);
    AddKeyToMap(T);
    AddKeyToMap(U);
    AddKeyToMap(V);
    AddKeyToMap(W);
    AddKeyToMap(X);
    AddKeyToMap(Y);
    AddKeyToMap(Z);
    AddKeyToMap(LeftBracket);
    AddKeyToMap(Backslash);
    AddKeyToMap(RightBracket);
    AddKeyToMap(GraveAccent);

    /* Navigation / function */
    AddKeyToMap(Escape);
    AddKeyToMap(Enter);
    AddKeyToMap(Tab);
    AddKeyToMap(Backspace);
    AddKeyToMap(Insert);
    AddKeyToMap(Delete);
    AddKeyToMap(RightArrow);
    AddKeyToMap(LeftArrow);
    AddKeyToMap(DownArrow);
    AddKeyToMap(UpArrow);
    AddKeyToMap(PageUp);
    AddKeyToMap(PageDown);
    AddKeyToMap(Home);
    AddKeyToMap(End);
    AddKeyToMap(CapsLock);
    AddKeyToMap(ScrollLock);
    AddKeyToMap(NumberLock);
    AddKeyToMap(PrintScreen);
    AddKeyToMap(Pause);
    AddKeyToMap(F1);
    AddKeyToMap(F2);
    AddKeyToMap(F3);
    AddKeyToMap(F4);
    AddKeyToMap(F5);
    AddKeyToMap(F6);
    AddKeyToMap(F7);
    AddKeyToMap(F8);
    AddKeyToMap(F9);
    AddKeyToMap(F10);
    AddKeyToMap(F11);
    AddKeyToMap(F12);
    AddKeyToMap(F13);
    AddKeyToMap(F14);
    AddKeyToMap(F15);
    AddKeyToMap(F16);
    AddKeyToMap(F17);
    AddKeyToMap(F18);
    AddKeyToMap(F19);
    AddKeyToMap(F20);
    AddKeyToMap(F21);
    AddKeyToMap(F22);
    AddKeyToMap(F23);
    AddKeyToMap(F24);

    /* Number Pad */
    AddKeyToMap(NumPad0);
    AddKeyToMap(NumPad1);
    AddKeyToMap(NumPad2);
    AddKeyToMap(NumPad3);
    AddKeyToMap(NumPad4);
    AddKeyToMap(NumPad5);
    AddKeyToMap(NumPad6);
    AddKeyToMap(NumPad7);
    AddKeyToMap(NumPad8);
    AddKeyToMap(NumPad9);
    AddKeyToMap(NumPadDecimal);
    AddKeyToMap(NumPadDivide);
    AddKeyToMap(NumPadMultiply);
    AddKeyToMap(NumPadSubtract);
    AddKeyToMap(NumPadAdd);
    AddKeyToMap(NumPadEnter);

    /* Modifiers */
    AddKeyToMap(LeftShift);
    AddKeyToMap(LeftControl);
    AddKeyToMap(LeftAlt);
    AddKeyToMap(LeftSuper);
    AddKeyToMap(RightShift);
    AddKeyToMap(RightControl);
    AddKeyToMap(RightAlt);
    AddKeyToMap(RightSuper);

    /* Mouse */
    AddKeyToMap(LeftMouseButton);
    AddKeyToMap(RightMouseButton);
    AddKeyToMap(MiddleMouseButton);

    AddKeyToMap(ThumbMouseButtonOne);
    AddKeyToMap(ThumbMouseButtonTwo);
}

void CKeys::AddKeyToMap(const CKey& Key)
{
    CKey::m_KeyCodeToKeyMap.emplace(Key.GetKeyCode(), Key);
}

/*-----------------*/
/* -- Lifecycle -- */
/*-----------------*/

void CInput::Initialize()
{
    CKeys::InitializeKeys();
}

void CInput::BeginFrame()
{
    m_MouseState.PreviousX = m_MouseState.CurrentX;
    m_MouseState.PreviousY = m_MouseState.CurrentY;
    m_MouseState.ScrollX = 0.0f;
    m_MouseState.ScrollY = 0.0f;
    
    TransitionPressedKeys();
    TransitionPressedMouseButtons();
    PollGamepads();
}

void CInput::EndFrame()
{
    ClearReleasedKeys();
}

/*----------------*/
/* -- Keyboard -- */
/*----------------*/

bool CInput::GetKeyDown(const CKey& Key)
{
    const auto It = m_KeyData.find(static_cast<int32>(Key.GetKeyCode()));
    
    return It != m_KeyData.end() && It->second.CurrentKeyState == EKeyState::Pressed;
}

bool CInput::GetKeyHeld(const CKey& Key)
{
    const auto It = m_KeyData.find(static_cast<int32>(Key.GetKeyCode()));
    
    return It != m_KeyData.end() && (It->second.CurrentKeyState == EKeyState::Pressed || It->second.CurrentKeyState == EKeyState::Held);
}

bool CInput::GetKeyUp(const CKey& Key)
{
    const auto It = m_KeyData.find(static_cast<int32>(Key.GetKeyCode()));
    
    return It != m_KeyData.end() && It->second.CurrentKeyState == EKeyState::Released;
}

/*-------------*/
/* -- Mouse -- */
/*-------------*/

bool CInput::GetMouseButtonDown(const CKey& MouseButton)
{
    const auto It = m_MouseButtonData.find(static_cast<int32>(MouseButton.GetKeyCode()));
    
    return It != m_MouseButtonData.end() && It->second.CurrentState == EKeyState::Pressed;
}

bool CInput::GetMouseButtonHeld(const CKey& MouseButton)
{
    const auto It = m_MouseButtonData.find(static_cast<int32>(MouseButton.GetKeyCode()));
    
    return It != m_MouseButtonData.end() && (It->second.CurrentState == EKeyState::Pressed || It->second.CurrentState == EKeyState::Held);
}

bool CInput::GetMouseButtonUp(const CKey& MouseButton)
{
    const auto It = m_MouseButtonData.find(static_cast<int32>(MouseButton.GetKeyCode()));
    
    return It != m_MouseButtonData.end() && It->second.CurrentState == EKeyState::Released;
}

void CInput::SetCursorMode(ECursorMode NewMode)
{
    auto* Window = static_cast<GLFWwindow*>(CApplication::GetInstance().GetWindow().GetHandle());

    switch (NewMode)
    {
        case ECursorMode::Normal: glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
        case ECursorMode::Hidden: glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); break;
        case ECursorMode::Locked: glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); break;
    }

    m_CursorMode = NewMode;
}

/*----------------*/
/* -- Gamepads -- */
/*----------------*/

std::string_view CInput::GetGamepadName(int32 GamepadIndex)
{
    if (!IsValidGamepadIndex(GamepadIndex))
        return "Invalid Gamepad";
    
    return m_GamepadStates[GamepadIndex].bConnected ? m_GamepadStates[GamepadIndex].Name : "Disconnected";
}

bool CInput::GetGamepadButtonDown(int32 GamepadIndex, EGamepadButton Button)
{
    if (!IsValidGamepadIndex(GamepadIndex) || !m_GamepadStates[GamepadIndex].bConnected)
        return false;
    
    return m_GamepadStates[GamepadIndex].GetButtonState(Button) == EKeyState::Pressed;
}

bool CInput::GetGamepadButtonHeld(int32 GamepadIndex, EGamepadButton Button)
{
    if (!IsValidGamepadIndex(GamepadIndex) || !m_GamepadStates[GamepadIndex].bConnected)
        return false;
    
    const EKeyState State = m_GamepadStates[GamepadIndex].GetButtonState(Button);
    
    return State == EKeyState::Pressed || State == EKeyState::Held;
}

bool CInput::GetGamepadButtonUp(int32 GamepadIndex, EGamepadButton Button)
{
    if (!IsValidGamepadIndex(GamepadIndex) || !m_GamepadStates[GamepadIndex].bConnected)
        return false;
    
    return m_GamepadStates[GamepadIndex].GetButtonState(Button) == EKeyState::Released;
}

float CInput::GetGamepadAxis(int32 GamepadIndex, EGamepadAxis Axis, float DeadZone)
{
    if (!IsValidGamepadIndex(GamepadIndex) || !m_GamepadStates[GamepadIndex].bConnected)
        return 0.0f;
    
    return ApplyDeadZone(m_GamepadStates[GamepadIndex].GetRawAxis(Axis), DeadZone);
}

/*-----------------------*/
/* -- Action Mappings -- */
/*-----------------------*/

bool CInput::GetAction(const std::string& ActionName)
{
    const auto It = m_ActionBindings.find(ActionName);
    if (It == m_ActionBindings.end())
        return false;

    for (const CInputActionBinding& Binding : It->second)
    {
        bool bFires = false;

        if (Binding.SourceType == CInputActionBinding::EActionSourceType::Key)
        {
            switch (Binding.Trigger)
            {
                case EInputActionTrigger::Pressed: bFires = GetKeyDown(Binding.Key); break;
                case EInputActionTrigger::Released: bFires = GetKeyUp(Binding.Key); break;
                case EInputActionTrigger::Held: bFires = GetKeyHeld(Binding.Key); break;
            }
        }
        else
        {
            switch (Binding.Trigger)
            {
                case EInputActionTrigger::Pressed: bFires = GetGamepadButtonDown(Binding.GamepadIndex, Binding.GamepadButton); break;
                case EInputActionTrigger::Released: bFires = GetGamepadButtonUp(Binding.GamepadIndex, Binding.GamepadButton); break;
                case EInputActionTrigger::Held: bFires = GetGamepadButtonHeld(Binding.GamepadIndex, Binding.GamepadButton); break;
            }
        }

        if (bFires)
            return true;
    }

    return false;
}

/*---------------------*/
/* -- Axis Mappings -- */
/*---------------------*/

float CInput::GetAxis(const std::string& AxisName)
{
    const auto It = m_AxisBindings.find(AxisName);
    if (It == m_AxisBindings.end())
        return 0.0f;

    float Value = 0.0f;

    for (const CInputAxisBinding& Binding : It->second)
    {
        if (Binding.SourceType == CInputAxisBinding::EActionSourceType::Key)
        {
            if (GetKeyHeld(Binding.Key))
                Value += Binding.Scale;
        }
        else // GamepadAxis
        {
            Value += ApplyDeadZone(GetGamepadAxis(Binding.GamepadIndex, Binding.GamepadAxis, Binding.DeadZone), 0.0f) * Binding.AxisScale;
        }
    }

    return Value;
}

/*--------------------------*/
/* -- Internal Functions -- */
/*--------------------------*/

void CInput::UpdateKeyState(const CKey& KeyToUpdate, EKeyState NewState)
{
    const int32 Code = static_cast<int32>(KeyToUpdate.GetKeyCode());
    auto& [Key, CurrentState, OldState] = m_KeyData[Code];
    
    Key = KeyToUpdate;
    OldState = CurrentState;
    CurrentState = NewState;
}

void CInput::UpdateMouseButtonState(const CKey& MouseButtonToUpdate, EKeyState NewState)
{
    const int32 Code = static_cast<int32>(MouseButtonToUpdate.GetKeyCode());
    auto& [MouseButton, CurrentState, OldState] = m_MouseButtonData[Code];
    
    MouseButton = MouseButtonToUpdate;
    OldState = CurrentState;
    CurrentState = NewState;
}

void CInput::OnMouseMoved(float NewXPosition, float NewYPosition)
{
    m_MouseState.CurrentX = NewXPosition;
    m_MouseState.CurrentY = NewYPosition;
}

void CInput::OnMouseScrolled(float XOffset, float YOffset)
{
    m_MouseState.ScrollX += XOffset;
    m_MouseState.ScrollY += YOffset;
}

void CInput::TransitionPressedKeys()
{
    for (auto& [Code, Data] : m_KeyData)
    {
        if (Data.CurrentKeyState == EKeyState::Pressed)
            UpdateKeyState(CKey::GetKeyFromKeyCode(static_cast<uint32>(Code)), EKeyState::Held);
    }
}

void CInput::TransitionPressedMouseButtons()
{
    for (auto& [Code, Data] : m_MouseButtonData)
    {
        if (Data.CurrentState == EKeyState::Pressed)
            UpdateMouseButtonState(CKey::GetKeyFromKeyCode(static_cast<uint32>(Code)), EKeyState::Held);
    }
}

void CInput::ClearReleasedKeys()
{
    for (auto& [Code, Data] : m_KeyData)
    {
        if (Data.CurrentKeyState == EKeyState::Released)
            UpdateKeyState(CKey::GetKeyFromKeyCode(static_cast<uint32>(Code)), EKeyState::Unknown);
    }

    for (auto& [Code, Data] : m_MouseButtonData)
    {
        if (Data.CurrentState == EKeyState::Released)
            UpdateMouseButtonState(CKey::GetKeyFromKeyCode(static_cast<uint32>(Code)), EKeyState::Unknown);
    }
}

void CInput::PollGamepads()
{
    for (int32 i = 0; i < GMaxGamepadCount; ++i)
    {
        CGamepadState& State = m_GamepadStates[i];
        const bool bWasConnected = State.bConnected;

        // glfwJoystickIsGamepad checks if the joystick has a valid SDL controller mapping (most modern controllers do).
        // Raw joysticks without a mapping will return false here. Possibly add support for those controllers?
        GLFWgamepadstate GLFWGamepadState = {};
        State.bConnected = glfwJoystickIsGamepad(i) && glfwGetGamepadState(i, &GLFWGamepadState);

        if (!State.bConnected)
        {
            if (bWasConnected)
            {
                ENGINE_LOG_INFO_TAG("Input", "Gamepad {} ('{}') disconnected.", i, State.Name);
                State = CGamepadState {};
            }
            
            continue;
        }

        if (!bWasConnected)
        {
            State.Name = glfwGetGamepadName(i);
            ENGINE_LOG_INFO_TAG("Input", "Gamepad {} ('{}') connected.", i, State.Name);
        }

        // ---- Button edge detection ----
        for (int32 Button = 0; Button < CGamepadState::ButtonCount; ++Button)
        {
            EKeyState& ButtonState = State.ButtonStates[Button];

            if (GLFWGamepadState.buttons[Button] == GLFW_PRESS)
            {
                // If it wasn't down last frame, mark Pressed; otherwise Held.
                ButtonState = (ButtonState == EKeyState::Unknown || ButtonState == EKeyState::Released) ? EKeyState::Pressed : EKeyState::Held;
            }
            else
            {
                // If it was down last frame, mark Released; otherwise Unknown.
                ButtonState = (ButtonState == EKeyState::Pressed || ButtonState == EKeyState::Held) ? EKeyState::Released : EKeyState::Unknown;
            }
        }

        // ---- Axis values ----
        for (int32 Index = 0; Index < CGamepadState::AxisCount; ++Index)
            State.RawAxisValues[Index] = GLFWGamepadState.axes[Index];

        // Normalize trigger range from GLFW's [-1, +1] to [0, 1].
        constexpr int32 LeftTrigger = static_cast<int32>(EGamepadAxis::LeftTrigger);
        constexpr int32 RightTrigger = static_cast<int32>(EGamepadAxis::RightTrigger);
        State.RawAxisValues[LeftTrigger] = (State.RawAxisValues[LeftTrigger] + 1.0f) * 0.5f;
        State.RawAxisValues[RightTrigger] = (State.RawAxisValues[RightTrigger] + 1.0f) * 0.5f;
    }
}

float CInput::ApplyDeadZone(float Value, float DeadZone)
{
    const float Abs = std::fabs(Value);
    if (Abs <= DeadZone)
        return 0.0f;

    // Rescale the remaining range so the output is still 0 to 1, not DeadZone to 1.
    const float Sign = (Value >= 0.0f) ? 1.0f : -1.0f;
    const float Rescaled = (Abs - DeadZone) / (1.0f - DeadZone);
    return Sign * Rescaled;
}
