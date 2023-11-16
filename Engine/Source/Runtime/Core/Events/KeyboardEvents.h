#pragma once

#include "Event.h"
#include "Core/CoreTypes.h"
#include "Input/CoreInputTypes.h"

#include <sstream>

class FKeyboardEvent : public FEvent
{
public:
    uint32 GetKey() const { return m_Key.GetKeyCode(); }

    SET_CLASS_EVENT_CATEGORY(InputCategory | KeyboardCategory)
protected:
    FKeyboardEvent(FKey Key)
        : m_Key(std::move(Key)) {}

    FKey m_Key;
};

class FKeyPressedEvent : public FKeyboardEvent
{
public:
    FKeyPressedEvent(const FKey& Key, bool bIsKeyHeldDown)
        : FKeyboardEvent(Key), bIsHeldDown(bIsKeyHeldDown) {}

    bool IsKeyHeldDown() const { return bIsHeldDown; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyPressedEvent: " << m_Key.GetDisplayName() << " (Held Down: " << (bIsHeldDown ? "True" : "False") << ")";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyPressed)
private:
    bool bIsHeldDown;
};

class FKeyReleasedEvent : public FKeyboardEvent
{
public:
    FKeyReleasedEvent(const FKey& Key)
        : FKeyboardEvent(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyReleasedEvent: " << m_Key.GetDisplayName();
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyReleased)
};

class FKeyTypedEvent : public FKeyboardEvent
{
public:
    // (Ayydan) We don't have a system for converting ASCII scancodes to regular keycodes. So, this just takes in the scancode.
    FKeyTypedEvent(const uint32 Key)
        : FKeyboardEvent(FKey()),  m_KeyCode(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyTypedEvent: " << m_KeyCode;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyTyped)
private:
    uint32 m_KeyCode;
};