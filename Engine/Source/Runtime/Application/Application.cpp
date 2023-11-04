#include "MoonlightPCH.h"
#include "Application.h"

#include "Events/ApplicationEvents.h"

extern bool bIsApplicationRunning;

FApplication::FApplication(const FApplicationSpecification& Specification)
    : m_ApplicationSpecification(Specification), m_CommandLineArguments(Specification.CommandLineArguments)
{
    m_ApplicationInstance = this;

    FLogging::Initialize();

    ENGINE_LOG_INFO_TAG("Core", "Initializing Moonlight Engine...");

    DispatchEvent<FApplicationInitializeEvent>();
}

FApplication::~FApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");

    // Clearing the event queue.
    // This also ensures that we free the memory used by it.
    std::queue<std::function<void()>>().swap(m_EventQueue);
    
    DispatchEvent<FApplicationShutdownEvent>();
}

void FApplication::OnEvent(FEvent& Event)
{
    FEventDispatcher EventDispatcher(Event);
}

void FApplication::Start()
{
    while (bIsRunning)
    {
        ProcessEvents();
    }
}

void FApplication::Restart()
{
    bIsRunning = false;
}

void FApplication::Close()
{
    bIsRunning = false;
    bIsApplicationRunning = false;
}

void FApplication::ProcessEvents()
{
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);

    while (!m_EventQueue.empty())
    {
        auto& EventFunction = m_EventQueue.front();
        EventFunction();

        m_EventQueue.pop();
    }
}

template<typename Event, typename ... EventArgs>
void FApplication::DispatchEvent(EventArgs&&... Arguments)
{
    static_assert(std::is_assignable_v<FEvent, Event>);

    std::shared_ptr<Event> EventInstance = std::make_shared<Event>(std::forward<EventArgs>(Arguments)...);
    OnEvent(*EventInstance);
}

template <typename EventFunction>
void FApplication::QueueEvent(EventFunction&& EventFunc)
{
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);
    m_EventQueue.push(EventFunc);
}
