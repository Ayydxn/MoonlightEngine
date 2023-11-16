#pragma once

#include "CoreInputTypes.h"
#include "Core/CoreTypes.h"

#include <map>
#include <utility>

class FInput
{
public:
    static void Initialize();

    /* -- Keyboard -- */
    static bool GetKeyDown(const FKey& Key);
    static bool GetKeyHeld(const FKey& Key);
    static bool GetKeyUp(const FKey& Key);

    /* -- Mouse -- */
    static bool GetMouseButtonDown(const FKey& MouseButton);
    static bool GetMouseButtonHeld(const FKey& MouseButton);
    static bool GetMouseButtonUp(const FKey& MouseButton);

    static float GetMouseX();
    static float GetMouseY();
    static std::pair<float, float> GetMousePosition();

    static ECursorMode GetCursorMode() { return m_CurrentCursorMode; }
    static void SetCursorMode(ECursorMode NewCursorMode);
private:
    /* -- Internal Functions -- */
    static void TransitionPressedKeys();
    static void TransitionPressedMouseButtons();

    static void UpdateKeyState(const FKey& Key, EKeyState NewState);
    static void UpdateMouseButtonState(const FKey& MouseButton, EKeyState NewState);

    static void ClearReleasedKeys();
private:
    inline static std::map<int32, FKeyData> m_KeyData;
    inline static std::map<int32, FMouseButtonData> m_MouseButtonData;

    inline static ECursorMode m_CurrentCursorMode = ECursorMode::Normal;

    friend FKeys;
    friend class FDesktopWindow;
    friend class FApplication;
};