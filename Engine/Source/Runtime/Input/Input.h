#pragma once

#include "CoreInputTypes.h"
#include "GamepadTypes.h"
#include "InputBindings.h"
#include "Core/CoreDefines.h"

#include <array>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class MOONLIGHT_API CInput
{
public:
    static void Initialize();
    static void BeginFrame();
    static void EndFrame();

    /*----------------*/
    /* -- Keyboard -- */
    /*----------------*/

    static bool GetKeyDown(const CKey& Key);
    static bool GetKeyHeld(const CKey& Key);
    static bool GetKeyUp(const CKey& Key);
    static bool IsShiftHeld() { return GetKeyHeld(CKeys::LeftShift) || GetKeyHeld(CKeys::RightShift); }
    static bool IsControlHeld() { return GetKeyHeld(CKeys::LeftControl) || GetKeyHeld(CKeys::RightControl); }
    static bool IsAltHeld() { return GetKeyHeld(CKeys::LeftAlt) || GetKeyHeld(CKeys::RightAlt); }

    /*-------------*/
    /* -- Mouse -- */
    /*-------------*/

    static bool GetMouseButtonDown(const CKey& MouseButton);
    static bool GetMouseButtonHeld(const CKey& MouseButton);
    static bool GetMouseButtonUp(const CKey& MouseButton);
    
    static float GetMouseX() { return m_MouseState.CurrentX; }
    static float GetMouseY() { return m_MouseState.CurrentY; }
    static std::pair<float, float> GetMousePosition() { return { m_MouseState.CurrentX, m_MouseState.CurrentY }; }
    static float GetMouseDeltaX() { return m_MouseState.GetDeltaX(); }
    static float GetMouseDeltaY() { return m_MouseState.GetDeltaY(); }
    static std::pair<float, float> GetMouseDelta() { return { m_MouseState.GetDeltaX(), m_MouseState.GetDeltaY() }; }
    
    static float GetScrollX() { return m_MouseState.ScrollX; }
    static float GetScrollY() { return m_MouseState.ScrollY; }

    static ECursorMode GetCursorMode() { return m_CursorMode; }
    static void SetCursorMode(ECursorMode NewMode);
    
    /*----------------*/
    /* -- Gamepads -- */
    /*----------------*/

    static bool IsGamepadConnected(int32 GamepadIndex) { return IsValidGamepadIndex(GamepadIndex) && m_GamepadStates[GamepadIndex].bConnected; }
    static std::string_view GetGamepadName(int32 GamepadIndex);

    static bool GetGamepadButtonDown(int32 GamepadIndex, EGamepadButton Button);
    static bool GetGamepadButtonHeld(int32 GamepadIndex, EGamepadButton Button);
    static bool GetGamepadButtonUp(int32 GamepadIndex, EGamepadButton Button);
    static float GetGamepadAxis(int32 GamepadIndex, EGamepadAxis Axis, float DeadZone = 0.15f);
    
    /*-----------------------*/
    /* -- Action Mappings -- */
    /*-----------------------*/

    static void RegisterAction(const std::string& ActionName, const CInputActionBinding& Binding) { m_ActionBindings[ActionName].push_back(Binding); }
    static void UnregisterAction(const std::string& ActionName) { m_ActionBindings.erase(ActionName); }
    static bool GetAction(const std::string& ActionName);
    
    /*---------------------*/
    /* -- Axis Mappings -- */
    /*---------------------*/

    static void RegisterAxis(const std::string& AxisName, const CInputAxisBinding& Binding) { m_AxisBindings[AxisName].push_back(Binding); }
    static void UnregisterAxis(const std::string& AxisName) { m_AxisBindings.erase(AxisName); }
    static float GetAxis(const std::string& AxisName);
private:
    /*--------------------------*/
    /* -- Internal Functions -- */
    /*--------------------------*/

    static void UpdateKeyState(const CKey& KeyToUpdate, EKeyState NewState);
    static void UpdateMouseButtonState(const CKey& MouseButtonToUpdate, EKeyState NewState);
    static void OnMouseMoved(float NewXPosition, float NewYPosition);
    static void OnMouseScrolled(float XOffset, float YOffset);

    static void TransitionPressedKeys();
    static void TransitionPressedMouseButtons();
    static void ClearReleasedKeys();

    static void PollGamepads();
    static bool IsValidGamepadIndex(int32 Index) { return Index >= 0 && Index < GMaxGamepadCount; }
    static float ApplyDeadZone(float Value, float DeadZone);
private:
    /*-------------*/
    /* -- State -- */
    /*-------------*/
    
    inline static std::unordered_map<int32, CKeyData> m_KeyData;
    inline static std::unordered_map<int32, CMouseButtonData> m_MouseButtonData;
    inline static CMouseState m_MouseState;
    inline static ECursorMode m_CursorMode = ECursorMode::Normal;

    inline static std::array<CGamepadState, GMaxGamepadCount> m_GamepadStates = {};

    inline static std::unordered_map<std::string, std::vector<CInputActionBinding>> m_ActionBindings;
    inline static std::unordered_map<std::string, std::vector<CInputAxisBinding>> m_AxisBindings;

    friend CKeys;
    friend class CDesktopWindow;
    friend class CApplication;
};
