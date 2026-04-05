#pragma once

#include "CoreInputTypes.h"
#include "GamepadTypes.h"

/*===========================================================================
Binding descriptors used by the Action and Axis mapping systems.

* CInputActionBinding  — Maps one source (key OR gamepad button) to a named action with a specific trigger condition.
* CInputAxisBinding — Maps one source (key OR gamepad axis) to a named axis with a scale and optional dead zone.

Both types use named static factories instead of direct construction so call-sites read clearly:
    CInput::RegisterAction("Jump", CInputActionBinding::FromKey(CKeys::Space));
    CInput::RegisterAction("Jump", CInputActionBinding::FromGamepadButton(0, EGamepadButton::A));

    CInput::RegisterAxis("MoveForward", CInputAxisBinding::FromKey(CKeys::W,  +1.0f));
    CInput::RegisterAxis("MoveForward", CInputAxisBinding::FromKey(CKeys::S,  -1.0f));
    CInput::RegisterAxis("MoveForward", CInputAxisBinding::FromGamepadAxis(0, EGamepadAxis::LeftStickY, -1.0f));
===========================================================================*/


enum class EInputActionTrigger : uint8
{
    Pressed,
    Released,
    Held
};

struct CInputActionBinding
{
    enum class EActionSourceType : uint8 { Key, GamepadButton };

    EActionSourceType SourceType = EActionSourceType::Key;
    EInputActionTrigger Trigger = EInputActionTrigger::Pressed;

    // Key source
    CKey Key;

    // Gamepad button source
    int32 GamepadIndex = 0;
    EGamepadButton GamepadButton = EGamepadButton::A;
    
    /*-----------------*/
    /* -- Factories -- */
    /*-----------------*/

    static CInputActionBinding FromKey(const CKey& Key, EInputActionTrigger Trigger = EInputActionTrigger::Pressed)
    {
        CInputActionBinding InputActionBinding;
        InputActionBinding.SourceType = EActionSourceType::Key;
        InputActionBinding.Key = Key;
        InputActionBinding.Trigger = Trigger;
        
        return InputActionBinding;
    }

    static CInputActionBinding FromGamepadButton(int32 GamepadIndex, EGamepadButton Button, EInputActionTrigger Trigger = EInputActionTrigger::Pressed)
    {
        CInputActionBinding InputActionBinding;
        InputActionBinding.SourceType = EActionSourceType::GamepadButton;
        InputActionBinding.GamepadIndex = GamepadIndex;
        InputActionBinding.GamepadButton = Button;
        InputActionBinding.Trigger = Trigger;
        
        return InputActionBinding;
    }
};


/*---------------------------------------------------------------------------
    CInputAxisBinding
---------------------------------------------------------------------------*/

struct CInputAxisBinding
{
    enum class EActionSourceType : uint8 { Key, GamepadAxis };

    EActionSourceType SourceType = EActionSourceType::Key;

    // Key source — contributes Scale when held, 0 when not.
    CKey Key;
    float Scale = 1.0f;

    // Gamepad axis source — contributes RawAxis * AxisScale (after dead zone).
    int32 GamepadIndex = 0;
    EGamepadAxis GamepadAxis = EGamepadAxis::LeftStickX;
    float AxisScale = 1.0f;
    float DeadZone = 0.15f;

    /* -- Factories -------------------------------------------------------- */

    static CInputAxisBinding FromKey(const CKey& Key, float Scale = 1.0f)
    {
        CInputAxisBinding InputAxisBinding;
        InputAxisBinding.SourceType = EActionSourceType::Key;
        InputAxisBinding.Key = Key;
        InputAxisBinding.Scale = Scale;
        
        return InputAxisBinding;
    }

    static CInputAxisBinding FromGamepadAxis(int32 GamepadIndex, EGamepadAxis Axis, float Scale = 1.0f, float DeadZone = 0.15f)
    {
        CInputAxisBinding InputAxisBinding;
        InputAxisBinding.SourceType = EActionSourceType::GamepadAxis;
        InputAxisBinding.GamepadIndex = GamepadIndex;
        InputAxisBinding.GamepadAxis = Axis;
        InputAxisBinding.AxisScale = Scale;
        InputAxisBinding.DeadZone = DeadZone;
        
        return InputAxisBinding;
    }
};
