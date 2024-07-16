#pragma once

#include "Event.h"

#include <sstream>

class CApplicationInitializeEvent : public IEvent
{
public:
    CApplicationInitializeEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationInitializeEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationInitialize)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class CApplicationUpdateEvent : public IEvent
{
public:
    CApplicationUpdateEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationUpdateEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationUpdate)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class CApplicationRenderEvent : public IEvent
{
public:
    CApplicationRenderEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationRenderEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationRender)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class CApplicationTickEvent : public IEvent
{
public:
    CApplicationTickEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationTickEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationTick)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class CApplicationShutdownEvent : public IEvent
{
public:
    CApplicationShutdownEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationShutdownEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationShutdown)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};