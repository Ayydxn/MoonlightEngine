#pragma once

#include "Event.h"
#include "Core/CoreTypes.h"

#include <sstream>

class FKeyboardEvent : public FEvent
{
public:
    uint32 GetKey() const { return m_Key; }

    SET_CLASS_EVENT_CATEGORY(InputCategory | KeyboardCategory)
protected:
    FKeyboardEvent(const uint32 Key)
        : m_Key(Key) {}

    uint32 m_Key;
};

class FKeyPressedEvent : public FKeyboardEvent
{
public:
    FKeyPressedEvent(uint32 Key, bool bIsKeyHeldDown)
        : FKeyboardEvent(Key), bIsHeldDown(bIsKeyHeldDown) {}

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

class FKeyReleasedEvent : public FKeyboardEvent
{
public:
    FKeyReleasedEvent(const uint32 Key)
        : FKeyboardEvent(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyReleasedEvent: " << m_Key;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyReleased)
};

class FKeyTypedEvent : public FKeyboardEvent
{
public:
    FKeyTypedEvent(const uint32 Key)
        : FKeyboardEvent(Key) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "KeyTypedEvent: " << m_Key;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(KeyTyped)
};