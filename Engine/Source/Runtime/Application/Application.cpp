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

    CWindowSpecification ApplicationWindowSpecification;
    ApplicationWindowSpecification.Title = m_ApplicationSpecification.Name;
    ApplicationWindowSpecification.Width = m_ApplicationSpecification.WindowWidth;
    ApplicationWindowSpecification.Height = m_ApplicationSpecification.WindowHeight;
    ApplicationWindowSpecification.WindowMode = m_ApplicationSpecification.WindowMode;
    ApplicationWindowSpecification.bEnableVSync = m_ApplicationSpecification.bEnableVSync;
    ApplicationWindowSpecification.bEnableDecoration = m_ApplicationSpecification.bEnableWindowDecoration;
    ApplicationWindowSpecification.bEnableResizing = m_ApplicationSpecification.bEnableWindowResizing;

    m_ApplicationWindow = IWindow::Create(ApplicationWindowSpecification);
    m_ApplicationWindow->Initialize();
    m_ApplicationWindow->SetEventCallbackFunction([this](IEvent& Event) { return OnEvent(Event); });
    
    DispatchEvent<CApplicationInitializeEvent>();
}

CApplication::~CApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");

    m_ApplicationWindow->SetEventCallbackFunction(nullptr);

    DispatchEvent<CApplicationShutdownEvent>();
}

void CApplication::Start()
{
    while (bIsRunning)
    {
        ProcessEvents();

        DispatchEvent<CApplicationUpdateEvent>();

        if (!bIsWindowMinizmied)
        {
            // TODO: (Ayydxn) Rendering operations go here in between an update and tick.

          DispatchEvent<CApplicationRenderEvent>();

            m_ApplicationWindow->SwapBuffers();
        }
        
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
    EventDispatcher.Dispatch<CWindowCloseEvent>([this](const CWindowCloseEvent&) { return OnWindowClose(); });
    EventDispatcher.Dispatch<CWindowMinimizeEvent>([this](const CWindowMinimizeEvent& WindowMinimizeEvent) { return OnWindowMinimized(WindowMinimizeEvent); });
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
    m_ApplicationWindow->ProcessEvents();
    
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);

    while (!m_EventQueue.empty())
    {
        auto& EventFunction = m_EventQueue.front();
        EventFunction();

        m_EventQueue.pop();
    }
}

bool CApplication::OnWindowClose()
{
    Close();

    return true;
}

bool CApplication::OnWindowMinimized(const CWindowMinimizeEvent& WindowMinimizeEvent)
{
    bIsWindowMinizmied = WindowMinimizeEvent.IsWindowMinimized();

    return true;
}
