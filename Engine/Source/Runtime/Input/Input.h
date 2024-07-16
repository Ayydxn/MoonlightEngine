#pragma once

#include "CoreInputTypes.h"
#include "Core/CoreDefines.h"

class MOONLIGHT_API CInput
{
public:
    static void Initialize();

    /* -- Keyboard -- */
    static bool GetKeyDown(const CKey& Key);
    static bool GetKeyHeld(const CKey& Key);
    static bool GetKeyUp(const CKey& Key);

    /* -- Mouse -- */
    static bool GetMouseButtonDown(const CKey& MouseButton);
    static bool GetMouseButtonHeld(const CKey& MouseButton);
    static bool GetMouseButtonUp(const CKey& MouseButton);

    static float GetMouseX();
    static float GetMouseY();
    static std::pair<float, float> GetMousePosition();

    static ECursorMode GetCursorMode() { return m_CurrentCursorMode; }
    static void SetCursorMode(ECursorMode NewCursorMode);
private:
    /* -- Internal Functions -- */
    static void TransitionPressedKeys();
    static void TransitionPressedMouseButtons();

    static void UpdateKeyState(const CKey& Key, EKeyState NewState);
    static void UpdateMouseButtonState(const CKey& MouseButton, EKeyState NewState);

    static void ClearReleasedKeys();
private:
    inline static std::map<int32, CKeyData> m_KeyData;
    inline static std::map<int32, CMouseButtonData> m_MouseButtonData;

    inline static ECursorMode m_CurrentCursorMode = ECursorMode::Normal;

    friend CKeys;
    friend class CDesktopWindow;
    friend class CApplication;
};
