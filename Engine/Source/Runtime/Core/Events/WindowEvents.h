#pragma once

#include "Event.h"

#include <sstream>

class CWindowCloseEvent : public IEvent
{
public:
    CWindowCloseEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "WindowCloseEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(WindowClose)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class CWindowMinimizeEvent : public IEvent
{
public:
    CWindowMinimizeEvent(const bool bIsWindowMinimized)
        : bIsMinimized(bIsWindowMinimized) {}

    bool IsWindowMinimized() const { return bIsMinimized; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "WindowMinimizeEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(WindowMinimize)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
private:
    bool bIsMinimized = false;
};

class CWindowResizeEvent : public IEvent
{
public:
    CWindowResizeEvent(uint32 NewWindowWidth, uint32 NewWindowHeight)
        : m_NewWindowWidth(NewWindowWidth), m_NewWindowHeight(NewWindowHeight) {}

    uint32 GetWidth() const { return m_NewWindowWidth; }
    uint32 GetHeight() const { return m_NewWindowHeight; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "WindowResizeEvent: (Width: " << m_NewWindowWidth << ", Height: " << m_NewWindowHeight << ")";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(WindowResize)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
private:
    uint32 m_NewWindowWidth = 0;
    uint32 m_NewWindowHeight = 0;
};

class CWindowMovedEvent : public IEvent
{
public:
    CWindowMovedEvent(int32 NewXPosition, int32 NewYPosition)
        : m_NewXPosition(NewXPosition), m_NewYPosition(NewYPosition) {}

    int32 GetXPosition() const { return m_NewXPosition; }
    int32 GetYPosition() const { return m_NewYPosition; }

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "WindowMovedEvent: (X: " << m_NewXPosition << ", Y:" << m_NewYPosition << ")";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(WindowMoved)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
private:
    int32 m_NewXPosition = 0;
    int32 m_NewYPosition = 0;
};

class CWindowFocusEvent : public IEvent
{
public:
    CWindowFocusEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "WindowFocusEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(WindowFocus)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class CWindowLostFocusEvent : public IEvent
{
public:
    CWindowLostFocusEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "WindowLostFocusEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(WindowLostFocus)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};