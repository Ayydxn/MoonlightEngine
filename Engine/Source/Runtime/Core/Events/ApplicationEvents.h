#pragma once

#include "Event.h"

#include <sstream>

class FApplicationInitializeEvent : public FEvent
{
public:
    FApplicationInitializeEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationInitializeEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationInitialize)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class FApplicationUpdateEvent : public FEvent
{
public:
    FApplicationUpdateEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationUpdateEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationUpdate)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class FApplicationRenderEvent : public FEvent
{
public:
    FApplicationRenderEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationRenderEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationRender)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class FApplicationTickEvent : public FEvent
{
public:
    FApplicationTickEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationTickEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationTick)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};

class FApplicationShutdownEvent : public FEvent
{
public:
    FApplicationShutdownEvent() = default;

    std::string ToString() const override
    {
        std::stringstream StringStream;
        StringStream << "ApplicationShutdownEvent";
        return StringStream.str();
    }

    SET_CLASS_EVENT_TYPE(ApplicationShutdown)
    SET_CLASS_EVENT_CATEGORY(ApplicationCategory)
};