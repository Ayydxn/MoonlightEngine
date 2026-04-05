#pragma once

#include "Core/CoreTypes.h"

#include <map>
#include <string>

/*===========================================================================
Fundamental types shared across the entire input system:
    - EKeyState         : The lifecycle state of any button
    - CKey              : A named, code-keyed input token
    - CKeyData          : Per-key frame state
    - CMouseButtonData  : Per-mouse-button frame state
    - CMouseState       : Aggregated mouse position / delta / scroll
    - ECursorMode       : Cursor visibility / lock modes
    - CKeys             : Static registry of all keyboard + mouse keys
===========================================================================*/

enum class EKeyState : uint8
{
    Unknown,
    Pressed,
    Held,
    Released
};

struct CKey
{
    CKey() = default;

    CKey(std::string KeyName, uint32 KeyCode)
        : m_KeyName(std::move(KeyName)), m_KeyCode(KeyCode) {}

    const std::string& GetDisplayName() const { return m_KeyName; }
    uint32 GetKeyCode() const { return m_KeyCode; }

    bool IsValid() const { return m_KeyCode != InvalidKeyCode; }

    bool operator==(const CKey& Other) const { return m_KeyCode == Other.m_KeyCode; }
    bool operator!=(const CKey& Other) const { return m_KeyCode != Other.m_KeyCode; }

    static constexpr uint32 InvalidKeyCode = 0xFFFFFFFF;

private:
    static CKey GetKeyFromKeyCode(uint32 KeyCode);

private:
    inline static std::map<uint32, CKey> m_KeyCodeToKeyMap;

    std::string m_KeyName = "Unknown Key";
    uint32 m_KeyCode = InvalidKeyCode;

    friend class CDesktopWindow;
    friend class CInput;
    friend struct CKeys;
};

struct CKeyData
{
    CKey Key;
    EKeyState CurrentKeyState = EKeyState::Unknown;
    EKeyState OldKeyState = EKeyState::Unknown;
};

struct CMouseButtonData
{
    CKey MouseButton;
    EKeyState CurrentState = EKeyState::Unknown;
    EKeyState OldState = EKeyState::Unknown;
};

struct CMouseState
{
    float CurrentX = 0.0f;
    float CurrentY = 0.0f;
    float PreviousX = 0.0f;
    float PreviousY = 0.0f;

    float ScrollX = 0.0f;
    float ScrollY = 0.0f;

    float GetDeltaX() const { return CurrentX - PreviousX; }
    float GetDeltaY() const { return CurrentY - PreviousY; }
};

enum class ECursorMode : uint8
{
    Normal,
    Hidden,
    Locked
};

struct CKeys
{
    inline static const CKey Space = CKey("Space", 0x20);
    inline static const CKey Apostrophe = CKey("Apostrophe", 0x27);
    inline static const CKey Comma = CKey("Comma", 0x2C);
    inline static const CKey Minus = CKey("Minus", 0x2D);
    inline static const CKey Period = CKey("Period", 0x2E);
    inline static const CKey Slash = CKey("Slash", 0x2F);

    inline static const CKey Zero = CKey("Zero", 0x30);
    inline static const CKey One = CKey("One", 0x31);
    inline static const CKey Two = CKey("Two", 0x32);
    inline static const CKey Three = CKey("Three", 0x33);
    inline static const CKey Four = CKey("Four", 0x34);
    inline static const CKey Five = CKey("Five", 0x35);
    inline static const CKey Six = CKey("Six", 0x36);
    inline static const CKey Seven = CKey("Seven", 0x37);
    inline static const CKey Eight = CKey("Eight", 0x38);
    inline static const CKey Nine = CKey("Nine", 0x39);

    inline static const CKey Semicolon = CKey("Semicolon", 0x3B);
    inline static const CKey Equal = CKey("Equal", 0x3D);

    inline static const CKey A = CKey("A", 0x41);
    inline static const CKey B = CKey("B", 0x42);
    inline static const CKey C = CKey("C", 0x43);
    inline static const CKey D = CKey("D", 0x44);
    inline static const CKey E = CKey("E", 0x45);
    inline static const CKey F = CKey("F", 0x46);
    inline static const CKey G = CKey("G", 0x47);
    inline static const CKey H = CKey("H", 0x48);
    inline static const CKey I = CKey("I", 0x49);
    inline static const CKey J = CKey("J", 0x4A);
    inline static const CKey K = CKey("K", 0x4B);
    inline static const CKey L = CKey("L", 0x4C);
    inline static const CKey M = CKey("M", 0x4D);
    inline static const CKey N = CKey("N", 0x4E);
    inline static const CKey O = CKey("O", 0x4F);
    inline static const CKey P = CKey("P", 0x50);
    inline static const CKey Q = CKey("Q", 0x51);
    inline static const CKey R = CKey("R", 0x52);
    inline static const CKey S = CKey("S", 0x53);
    inline static const CKey T = CKey("T", 0x54);
    inline static const CKey U = CKey("U", 0x55);
    inline static const CKey V = CKey("V", 0x56);
    inline static const CKey W = CKey("W", 0x57);
    inline static const CKey X = CKey("X", 0x58);
    inline static const CKey Y = CKey("Y", 0x59);
    inline static const CKey Z = CKey("Z", 0x5A);

    inline static const CKey LeftBracket = CKey("Left Bracket", 0x5B);
    inline static const CKey Backslash = CKey("Backslash", 0x5C);
    inline static const CKey RightBracket = CKey("Right Bracket", 0x5D);
    inline static const CKey GraveAccent = CKey("Grave Accent", 0x60);

    /* -----------------------------------------------------------------------
       Keyboard — Function and navigation keys
    ----------------------------------------------------------------------- */

    inline static const CKey Escape = CKey("Escape", 0x100);
    inline static const CKey Enter = CKey("Enter", 0x101);
    inline static const CKey Tab = CKey("Tab", 0x102);
    inline static const CKey Backspace = CKey("Backspace", 0x103);
    inline static const CKey Insert = CKey("Insert", 0x104);
    inline static const CKey Delete = CKey("Delete", 0x105);
    inline static const CKey RightArrow = CKey("Right Arrow", 0x106);
    inline static const CKey LeftArrow = CKey("Left Arrow", 0x107);
    inline static const CKey DownArrow = CKey("Down Arrow", 0x108);
    inline static const CKey UpArrow = CKey("Up Arrow", 0x109);
    inline static const CKey PageUp = CKey("Page Up", 0x10A);
    inline static const CKey PageDown = CKey("Page Down", 0x10B);
    inline static const CKey Home = CKey("Home", 0x10C);
    inline static const CKey End = CKey("End", 0x10D);
    inline static const CKey CapsLock = CKey("Caps Lock", 0x118);
    inline static const CKey ScrollLock = CKey("Scroll Lock", 0x119);
    inline static const CKey NumberLock = CKey("Number Lock", 0x11A);
    inline static const CKey PrintScreen = CKey("Print Screen", 0x11B);
    inline static const CKey Pause = CKey("Pause", 0x11C);

    inline static const CKey F1 = CKey("F1", 0x122);
    inline static const CKey F2 = CKey("F2", 0x123);
    inline static const CKey F3 = CKey("F3", 0x124);
    inline static const CKey F4 = CKey("F4", 0x125);
    inline static const CKey F5 = CKey("F5", 0x126);
    inline static const CKey F6 = CKey("F6", 0x127);
    inline static const CKey F7 = CKey("F7", 0x128);
    inline static const CKey F8 = CKey("F8", 0x129);
    inline static const CKey F9 = CKey("F9", 0x12A);
    inline static const CKey F10 = CKey("F10", 0x12B);
    inline static const CKey F11 = CKey("F11", 0x12C);
    inline static const CKey F12 = CKey("F12", 0x12D);
    inline static const CKey F13 = CKey("F13", 0x12E);
    inline static const CKey F14 = CKey("F14", 0x12F);
    inline static const CKey F15 = CKey("F15", 0x130);
    inline static const CKey F16 = CKey("F16", 0x131);
    inline static const CKey F17 = CKey("F17", 0x132);
    inline static const CKey F18 = CKey("F18", 0x133);
    inline static const CKey F19 = CKey("F19", 0x134);
    inline static const CKey F20 = CKey("F20", 0x135);
    inline static const CKey F21 = CKey("F21", 0x136);
    inline static const CKey F22 = CKey("F22", 0x137);
    inline static const CKey F23 = CKey("F23", 0x138);
    inline static const CKey F24 = CKey("F24", 0x139);

    /* -----------------------------------------------------------------------
       Keyboard — Numpad
    ----------------------------------------------------------------------- */

    inline static const CKey NumPad0 = CKey("NumPad 0", 0x140);
    inline static const CKey NumPad1 = CKey("NumPad 1", 0x141);
    inline static const CKey NumPad2 = CKey("NumPad 2", 0x142);
    inline static const CKey NumPad3 = CKey("NumPad 3", 0x143);
    inline static const CKey NumPad4 = CKey("NumPad 4", 0x144);
    inline static const CKey NumPad5 = CKey("NumPad 5", 0x145);
    inline static const CKey NumPad6 = CKey("NumPad 6", 0x146);
    inline static const CKey NumPad7 = CKey("NumPad 7", 0x147);
    inline static const CKey NumPad8 = CKey("NumPad 8", 0x148);
    inline static const CKey NumPad9 = CKey("NumPad 9", 0x149);
    inline static const CKey NumPadDecimal = CKey("NumPad Decimal", 0x14A);
    inline static const CKey NumPadDivide = CKey("NumPad Divide", 0x14B);
    inline static const CKey NumPadMultiply = CKey("NumPad Multiply", 0x14C);
    inline static const CKey NumPadSubtract = CKey("NumPad Subtract", 0x14D);
    inline static const CKey NumPadAdd = CKey("NumPad Add", 0x14E);
    inline static const CKey NumPadEnter = CKey("NumPad Enter", 0x14F);
    
    /*---------------------*/
    /* -- Modifier Keys -- */
    /*---------------------*/

    inline static const CKey LeftShift = CKey("Left Shift", 0x154);
    inline static const CKey LeftControl = CKey("Left Control", 0x155);
    inline static const CKey LeftAlt = CKey("Left Alt", 0x156);
    inline static const CKey LeftSuper = CKey("Left Super", 0x157);
    inline static const CKey RightShift = CKey("Right Shift", 0x158);
    inline static const CKey RightControl = CKey("Right Control", 0x159);
    inline static const CKey RightAlt = CKey("Right Alt", 0x15A);
    inline static const CKey RightSuper = CKey("Right Super", 0x15B);
    
    /*---------------------*/
    /* -- Mouse Buttons -- */
    /*---------------------*/

    inline static const CKey LeftMouseButton = CKey("Left Mouse Button", 0x0);
    inline static const CKey RightMouseButton = CKey("Left Mouse Button", 0x1);
    inline static const CKey MiddleMouseButton = CKey(" Middle Mouse Button", 0x2);
    inline static const CKey ThumbMouseButtonOne = CKey("Thumb Mouse Button One", 0x3);
    inline static const CKey ThumbMouseButtonTwo = CKey("Thumb Mouse Button Two", 0x4);
private:
    static void InitializeKeys();
    static void AddKeyToMap(const CKey& Key);

    friend class CInput;
};

inline CKey CKey::GetKeyFromKeyCode(uint32 KeyCode)
{
    const auto It = m_KeyCodeToKeyMap.find(KeyCode);

    return (It != m_KeyCodeToKeyMap.end()) ? It->second : CKey {};
}
