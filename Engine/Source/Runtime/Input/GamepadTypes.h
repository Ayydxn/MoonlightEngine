#pragma once

#include "Core/CoreTypes.h"
#include "CoreInputTypes.h"

#include <array>
#include <string>

/*===========================================================================
Types specific to gamepad / controller input:
    - EGamepadButton  : Logical button identifiers (matches GLFW layout)
    - EGamepadAxis    : Logical axis identifiers   (matches GLFW layout)
    - CGamepadState   : Per-controller frame state (polled each BeginFrame)

Gamepad input is polled, not event-driven.
GLFW doesn't provide gamepad callbacks, so CInput::BeginFrame() calls glfwGetGamepadState() for each of the GMaxGamepadCount slots and
builds EKeyState transitions from the difference between this frame and the previous frame.

Axis conventions:
    - Stick axes are in the range [-1, +1]. Positive X = right, Positive Y = down.
    - Trigger axes are normalised to [0, 1]  (GLFW raw range is [-1, +1]).
    - A configurable dead zone (default 0.15) is applied per-query in GetAxis().
===========================================================================*/

static constexpr int32 GMaxGamepadCount = 4;

enum class EGamepadButton : uint8
{
    // Xbox/Generic Controllers 
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    
    //PlayStation Controllers
    Cross = A,
    Circle = B,
    Square = X,
    Triangle = Y,
    
    LeftBumper = 4,
    RightBumper = 5,
    Back = 6,
    Start = 7,
    Guide = 8, // "Home" / Xbox button — may not be available on all controllers
    LeftThumbstick = 9, // Left stick click
    RightThumbstick = 10, // Right stick click
    DPadUp = 11,
    DPadRight = 12,
    DPadDown = 13,
    DPadLeft = 14
};

enum class EGamepadAxis : uint8
{
    LeftStickX = 0,
    LeftStickY = 1,
    RightStickX = 2,
    RightStickY = 3,
    LeftTrigger = 4,
    RightTrigger = 5
};

struct CGamepadState
{
    // (Ayydxn) Probably not hardcode them like this?
    static constexpr int32 ButtonCount = 15;
    static constexpr int32 AxisCount = 6;

    std::string Name;
    bool bConnected = false;
    std::array<EKeyState, ButtonCount> ButtonStates = {};
    std::array<float, AxisCount> RawAxisValues = {};
    
    EKeyState GetButtonState(EGamepadButton Button) const
    {
        return ButtonStates[static_cast<uint8>(Button)];
    }

    float GetRawAxis(EGamepadAxis Axis) const
    {
        return RawAxisValues[static_cast<uint8>(Axis)];
    }
};
