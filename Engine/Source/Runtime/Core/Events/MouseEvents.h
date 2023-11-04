#pragma once

#include "Event.h"
#include "Core/CoreTypes.h"

#include <sstream>

class FMouseButtonEvent : public FEvent
{
public:
    uint32 GetMouseButton() const { return m_MouseButton; }

    SET_CLASS_EVENT_CATEGORY(InputCategory | MouseCategory | MouseButtonCategory)
protected:
    FMouseButtonEvent(const uint32 MouseButton)
        : m_MouseButton(MouseButton) {}

    uint32 m_MouseButton;
};

class FMouseButtonPressedEvent : public FMouseButtonEvent
{
public:
    FMouseButtonPressedEvent(const uint32 MouseButton)
        : FMouseButtonEvent(MouseButton) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "MouseButtonPressedEvent: " << m_MouseButton;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(MouseButtonPressed)
};

class FMouseButtonReleasedEvent : public FMouseButtonEvent
{
public:
    FMouseButtonReleasedEvent(const uint32 MouseButton)
        : FMouseButtonEvent(MouseButton) {}

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "MouseButtonReleasedEvent: " << m_MouseButton;
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(MouseButtonReleased)
};

class FMouseMovedEvent : public FEvent
{
public:
    FMouseMovedEvent(float XPosition, float YPosition)
        : m_XPosition(XPosition), m_YPosition(YPosition) {}

    float GetXPosition() const { return m_XPosition; }
    float GetYPosition() const { return m_YPosition; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "MouseMovedEvent: (X: " << m_XPosition << ", Y: " << m_YPosition << ")";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(MouseMoved)
    SET_CLASS_EVENT_CATEGORY(InputCategory | MouseCategory)
private:
    float m_XPosition, m_YPosition;
};

class FMouseScrolledEvent : public FEvent
{
public:
    FMouseScrolledEvent(float XOffset, float YOffset)
        : m_XOffset(XOffset), m_YOffset(YOffset) {}

    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "MouseScrolledEvent: (X: " << m_XOffset << ", Y: " << m_YOffset << ")";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(MouseScrolled)
    SET_CLASS_EVENT_CATEGORY(InputCategory | MouseCategory)
private:
    float m_XOffset, m_YOffset;
};