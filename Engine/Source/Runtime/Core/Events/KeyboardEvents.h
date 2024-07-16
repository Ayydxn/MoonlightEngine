#pragma once

#include "Event.h"
#include "Core/CoreTypes.h"

#include <sstream>

class CKeyboardEvent : public IEvent
{
public:
    uint32 GetKey() const { return m_Key; }

    SET_CLASS_EVENT_CATEGORY(InputCategory | KeyboardCategory)
protected:
    explicit CKeyboardEvent(const uint32 Key)
        : m_Key(Key) {}

    uint32 m_Key;
};

class CKeyPressedEvent : public CKeyboardEvent
{
public:
    CKeyPressedEvent(uint32 Key, bool bIsKeyHeldDown)
        : CKeyboardEvent(Key), bIsHeldDown(bIsKeyHeldDown) {}

    bool IsKeyHeldDown() const { return bIsHeldDown; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyPressedEvent: " << m_Key << " (Held Down: " << (bIsHeldDown ? "True" : "False") << ")";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyPressed)
private:
    bool bIsHeldDown;
};

class CKeyReleasedEvent : public CKeyboardEvent
{
public:
    CKeyReleasedEvent(const uint32 Key)
        : CKeyboardEvent(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyReleasedEvent: " << m_Key;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyReleased)
};

class CKeyTypedEvent : public CKeyboardEvent
{
public:
    explicit CKeyTypedEvent(const uint32 Key)
        : CKeyboardEvent(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyTypedEvent: " << m_Key;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyTyped)
};