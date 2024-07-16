#pragma once

#include "Core/CoreTypes.h"

#include <map>
#include <string>

enum class EKeyState
{
    Unknown,

    Pressed,
    Released,
    Held
};

struct CKey
{
    CKey() = default;

    CKey(std::string KeyName, uint32 KeyCode)
        : m_KeyName(std::move(KeyName)), m_KeyCode(KeyCode) {}

    const std::string& GetDisplayName() const { return m_KeyName; }
    uint32 GetKeyCode() const { return m_KeyCode; }
private:
    static CKey GetKeyFromKeyCode(uint32 KeyCode) { return m_KeyCodeToKeyMap[KeyCode]; }
private:
    inline static std::map<uint32, CKey> m_KeyCodeToKeyMap;

    std::string m_KeyName = "Unknown Key";
    uint32 m_KeyCode = 0;

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

/*-------------*/
/* -- Mouse -- */
/*-------------*/

struct CMouseButtonData
{
    CKey MouseButton;
    EKeyState CurrentMouseButtonState = EKeyState::Unknown;
    EKeyState OldMouseButtonState = EKeyState::Unknown;
};

enum class ECursorMode
{
    Normal,
    Hidden,
    Locked
};

struct CKeys
{
    /*---------------------*/
    /* -- Keyboard Keys -- */
    /*---------------------*/

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

    inline static const CKey KeyPadZero = CKey("KeyPad Zero", 0x140);
    inline static const CKey KeyPadOne = CKey("KeyPad One", 0x141);
    inline static const CKey KeyPadTwo = CKey("KeyPad Two", 0x142);
    inline static const CKey KeyPadThree = CKey("KeyPad Three", 0x143);
    inline static const CKey KeyPadFour = CKey("KeyPad Four", 0x144);
    inline static const CKey KeyPadFive = CKey("KeyPad Five", 0x145);
    inline static const CKey KeyPadSix = CKey("KeyPad Six", 0x146);
    inline static const CKey KeyPadSeven = CKey("KeyPad Seven", 0x147);
    inline static const CKey KeyPadEight = CKey("KeyPad Eight", 0x148);
    inline static const CKey KeyPadNine = CKey("KeyPad Nine", 0x149);
    inline static const CKey KeyPadDecimal = CKey("KeyPad Decimal", 0x14A);
    inline static const CKey KeyPadDivide = CKey("KeyPad Divide", 0x14B);
    inline static const CKey KeyPadMultiply = CKey("KeyPad Multiply", 0x14C);
    inline static const CKey KeyPadSubtract = CKey("KeyPad Subtract", 0x14D);
    inline static const CKey KeyPadAdd = CKey("KeyPad Add", 0x14E);
    inline static const CKey KeyPadEnter = CKey("KeyPad Enter", 0x14F);

    inline static const CKey LeftShift = CKey("Left Shift", 0x154);
    inline static const CKey LeftControl = CKey("Left Control", 0x155);
    inline static const CKey LeftAlt = CKey("Left Alt", 0x156);
    inline static const CKey LeftSuper = CKey("Left Super", 0x157);

    inline static const CKey RightShift = CKey("Right Shift", 0x158);
    inline static const CKey RightControl = CKey("Right Control", 0x159);
    inline static const CKey RightAlt = CKey("Right Alt", 0x15A);
    inline static const CKey RightSuper = CKey("Right Super", 0x15B);

    /*-------------*/
    /* -- Mouse -- */
    /*-------------*/

    inline static const CKey LeftMouseButton = CKey("Left Mouse Button", 0x0);
    inline static const CKey RightMouseButton = CKey("Right Mouse Button", 0x1);
    inline static const CKey MiddleMouseButton = CKey("Middle Mouse Button", 0x2);

    inline static const CKey ThumbMouseButtonOne = CKey("Thumb Mouse Button One", 0x3);
    inline static const CKey ThumbMouseButtonTwo = CKey("Thumb Mouse Button Two", 0x4);
private:
    static void InitializeKeys()
    {
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

        AddKeyToMap(KeyPadZero);
        AddKeyToMap(KeyPadOne);
        AddKeyToMap(KeyPadTwo);
        AddKeyToMap(KeyPadThree);
        AddKeyToMap(KeyPadFour);
        AddKeyToMap(KeyPadFive);
        AddKeyToMap(KeyPadSix);
        AddKeyToMap(KeyPadSeven);
        AddKeyToMap(KeyPadEight);
        AddKeyToMap(KeyPadNine);
        AddKeyToMap(KeyPadDecimal);
        AddKeyToMap(KeyPadDivide);
        AddKeyToMap(KeyPadMultiply);
        AddKeyToMap(KeyPadSubtract);
        AddKeyToMap(KeyPadAdd);
        AddKeyToMap(KeyPadEnter);

        AddKeyToMap(LeftShift);
        AddKeyToMap(LeftControl);
        AddKeyToMap(LeftAlt);
        AddKeyToMap(LeftSuper);

        AddKeyToMap(RightShift);
        AddKeyToMap(RightControl);
        AddKeyToMap(RightAlt);
        AddKeyToMap(RightSuper);

        /*-------------*/
        /* -- Mouse -- */
        /*-------------*/

        AddKeyToMap(LeftMouseButton);
        AddKeyToMap(RightMouseButton);
        AddKeyToMap(MiddleMouseButton);

        AddKeyToMap(ThumbMouseButtonOne);
        AddKeyToMap(ThumbMouseButtonTwo);
    }

    static void AddKeyToMap(const CKey& Key)
    {
        // Don't add the key to map if it is already there.
        if (CKey::m_KeyCodeToKeyMap.contains(Key.GetKeyCode()))
            return;

        CKey::m_KeyCodeToKeyMap[Key.GetKeyCode()] = Key;
    }

    friend class CInput;
};