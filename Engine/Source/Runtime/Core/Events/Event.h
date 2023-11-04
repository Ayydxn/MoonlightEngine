#pragma once

#include <functional>
#include <string>

enum class EEventType
{
    Unknown,

    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
    WindowClose, WindowMinimize, WindowResize, WindowMoved, WindowFocus, WindowLostFocus,
    ApplicationInitialize, ApplicationUpdate, ApplicationRender, ApplicationTick, ApplicationShutdown
};

enum EEventCategory
{
    ApplicationCategory = 1u << 0,
    InputCategory       = 1u << 1,
    KeyboardCategory    = 1u << 2,
    MouseCategory       = 1u << 3,
    MouseButtonCategory = 1u << 4
};

#define SET_CLASS_EVENT_TYPE(Type) static EEventType GetStaticType() { return EEventType::Type; }\
                                   virtual EEventType GetEventType() const override { return GetStaticType(); }\
                                   virtual const char* GetName() const override { return #Type; }

#define SET_CLASS_EVENT_CATEGORY(Category) virtual int GetCategoryFlags() const override { return EEventCategory::Category; }

class FEvent
{
public:
    virtual ~FEvent() = default;

    virtual EEventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    bool IsInCategory(EEventCategory EventCategory) const
    {
        return GetCategoryFlags() & EventCategory;
    }

    bool bIsHandled = false;
};

class FEventDispatcher
{
    template<typename EventClass>
    using TEventFunction = std::function<bool(EventClass&)>;
public:
    FEventDispatcher(FEvent& Event)
        : m_Event(Event) {}

    template<typename EventClass>
    bool Dispatch(TEventFunction<EventClass> EventFunc)
    {
        if (m_Event.GetEventType() == EventClass::GetStaticType() && !m_Event.bIsHandled)
        {
            m_Event.bIsHandled = EventFunc(*static_cast<EventClass*>(&m_Event));
            return true;
        }

        return false;
    }
private:
    FEvent& m_Event;
};