#pragma once

#include "Event.h"
#include "Core/CoreTypes.h"
#include "Input/CoreInputTypes.h"

#include <sstream>

class CKeyboardEvent : public IEvent
{
public:
    uint32 GetKey() const { return m_Key.GetKeyCode(); }

    SET_CLASS_EVENT_CATEGORY(InputCategory | KeyboardCategory)
protected:
    explicit CKeyboardEvent(const CKey& Key)
        : m_Key(Key) {}

    CKey m_Key;
};

class CKeyPressedEvent : public CKeyboardEvent
{
public:
    CKeyPressedEvent(const CKey& Key, bool bIsKeyHeldDown)
        : CKeyboardEvent(Key), bIsHeldDown(bIsKeyHeldDown) {}

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

class CKeyReleasedEvent : public CKeyboardEvent
{
public:
    CKeyReleasedEvent(const CKey& Key)
        : CKeyboardEvent(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyReleasedEvent: " << m_Key.GetDisplayName();
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyReleased)
};

class CKeyTypedEvent : public CKeyboardEvent
{
public:
    explicit CKeyTypedEvent(const uint32 Key)
        : CKeyboardEvent(CKey()), m_KeyCode(Key) {}

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