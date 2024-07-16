#include "MoonlightPCH.h"
#include "Application.h"
#include "Events/ApplicationEvents.h"

CApplication* CApplication::m_ApplicationInstance = nullptr;

CApplication::CApplication(CApplicationSpecification ApplicationSpecification)
    : m_ApplicationSpecification(std::move(ApplicationSpecification))
{
    m_ApplicationInstance = this;
    
    CLogging::Initialize();
    
    ENGINE_LOG_INFO_TAG("Core", "Initializing Moonlight Engine...");

    DispatchEvent<CApplicationInitializeEvent>();
}

CApplication::~CApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");

    DispatchEvent<CApplicationShutdownEvent>();
}

void CApplication::Start()
{
    while (bIsRunning)
    {
        ProcessEvents();

        DispatchEvent<CApplicationUpdateEvent>();

        // TODO: (Ayydxn) Rendering operations go here in between an update and tick.
        
        DispatchEvent<CApplicationTickEvent>();
    }
}

void CApplication::Close()
{
    bIsRunning = false;
}

void CApplication::OnEvent(IEvent& Event)
{
    CEventDispatcher EventDispatcher(Event);
}

template<typename Event, typename ... EventArgs>
void CApplication::DispatchEvent(EventArgs&&... Arguments)
{
    static_assert(std::is_assignable_v<IEvent, Event>);

    std::shared_ptr<Event> EventInstance = std::make_shared<Event>(std::forward<EventArgs>(Arguments)...);
    OnEvent(*EventInstance);
}

template <typename EventFunction>
void CApplication::QueueEvent(EventFunction&& EventFunc)
{
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);
    m_EventQueue.push(EventFunc);
}

void CApplication::ProcessEvents()
{
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);

    while (!m_EventQueue.empty())
    {
        auto& EventFunction = m_EventQueue.front();
        EventFunction();

        m_EventQueue.pop();
    }
}