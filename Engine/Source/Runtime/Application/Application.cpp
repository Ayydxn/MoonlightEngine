#include "MoonlightPCH.h"
#include "Application.h"
#include "Core/Misc/CommandLineParser.h"
#include "Events/ApplicationEvents.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"

extern bool bIsApplicationRunning;

FApplication::FApplication(const FApplicationSpecification& Specification)
    : m_ApplicationSpecification(Specification), m_CommandLineArguments(Specification.CommandLineArguments)
{
    m_ApplicationInstance = this;

    FLogging::Initialize();

    ENGINE_LOG_INFO_TAG("Core", "Initializing Moonlight Engine...");

    FRenderer::PreInitialize();

    FWindowSpecification ApplicationWindowSpecification;
    ApplicationWindowSpecification.Title = m_ApplicationSpecification.Name;
    ApplicationWindowSpecification.Width = m_ApplicationSpecification.WindowWidth;
    ApplicationWindowSpecification.Height = m_ApplicationSpecification.WindowHeight;
    ApplicationWindowSpecification.WindowMode = m_ApplicationSpecification.WindowMode;
    ApplicationWindowSpecification.bEnableVSync = m_ApplicationSpecification.bEnableVSync;
    ApplicationWindowSpecification.bEnableDecoration = m_ApplicationSpecification.bEnableWindowDecoration;
    ApplicationWindowSpecification.bEnableResizing = m_ApplicationSpecification.bEnableWindowResizing;

    /*-----------------------------------------------------*/
    /* -- Parsing window related command line arguments -- */
    /*-----------------------------------------------------*/

    if (FCommandLineParser::Param(m_CommandLineArguments, "windowed"))
        ApplicationWindowSpecification.WindowMode = EWindowMode::Windowed;
    
    if (FCommandLineParser::Param(m_CommandLineArguments, "windowedFullscreen"))
        ApplicationWindowSpecification.WindowMode = EWindowMode::WindowedFullscreen;
    
    if (FCommandLineParser::Param(m_CommandLineArguments, "fullscreen"))
        ApplicationWindowSpecification.WindowMode = EWindowMode::Fullscreen;

    if (ApplicationWindowSpecification.WindowMode == EWindowMode::Windowed || ApplicationWindowSpecification.WindowMode == EWindowMode::Fullscreen)
    {
        int32 CmdLineWindowWidth = 0;
        int32 CmdLineWindowHeight = 0;

        if (FCommandLineParser::Value(m_CommandLineArguments, "ResolutionX", &CmdLineWindowWidth))
        {
            if (CmdLineWindowWidth != 0)
                ApplicationWindowSpecification.Width = CmdLineWindowWidth;
        }

        if (FCommandLineParser::Value(m_CommandLineArguments, "ResolutionY", &CmdLineWindowHeight))
        {
            if (CmdLineWindowHeight != 0)
                ApplicationWindowSpecification.Height = CmdLineWindowHeight;
        }
    }
    
    m_ApplicationWindow = FWindow::Create(ApplicationWindowSpecification);
    m_ApplicationWindow->Initialize();
    m_ApplicationWindow->SetEventCallbackFunction([this](FEvent& Event) { return OnEvent(Event); });

    FInput::Initialize();
    FRenderer::Initialize();

    DispatchEvent<FApplicationInitializeEvent>();
}

FApplication::~FApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");

    m_ApplicationWindow->SetEventCallbackFunction(nullptr);

    for (FLayer* Layer : m_LayerStack)
    {
        Layer->OnDetach();
        delete Layer;
    }

    // Clearing the event queue.
    // This also ensures that we free the memory used by it.
    std::queue<std::function<void()>>().swap(m_EventQueue);

    FRenderer::Shutdown();
}

void FApplication::OnEvent(FEvent& Event)
{
    FEventDispatcher EventDispatcher(Event);
    EventDispatcher.Dispatch<FWindowCloseEvent>([this](const FWindowCloseEvent&) { return OnWindowClose(); });
    EventDispatcher.Dispatch<FWindowMinimizeEvent>([this](const FWindowMinimizeEvent& WindowMinimizeEvent) { return OnWindowMinimize(WindowMinimizeEvent); });

    for (const auto& Layer : m_LayerStack)
    {
        Layer->OnEvent(Event);

        if (Event.bIsHandled)
            break;
    }
}

void FApplication::Start()
{
    while (bIsRunning)
    {
        OnInitialize();

        DispatchEvent<FApplicationInitializeEvent>();
        
        while (bIsRunning)
        {
            /*-----------------*/
            /* --  Updating -- */
            /*-----------------*/

            OnUpdate();

            for (FLayer* Layer : m_LayerStack)
                Layer->OnUpdate();

            ProcessEvents();

            DispatchEvent<FApplicationUpdateEvent>();

            /*-----------------*/
            /* -- Rendering -- */
            /*-----------------*/

            // Rendering will only happen if the window isn't minimized. (TODO: (Ayydan) The render loop should be executed on a separate render thread)
            if (!bIsWindowMinimized)
            {
                OnPreRender();

                for (FLayer* Layer : m_LayerStack)
                    Layer->OnPreRender();

                FRenderer::BeginFrame();

                OnRender();

                for (FLayer* Layer : m_LayerStack)
                    Layer->OnRender();

                DispatchEvent<FApplicationRenderEvent>();
            
                OnPostRender();

                for (FLayer* Layer : m_LayerStack)
                    Layer->OnPostRender();

                FRenderer::EndFrame();

                m_ApplicationWindow->SwapBuffers();
            }

            /*---------------*/
            /* -- Ticking -- */
            /*---------------*/

            OnTick();

            for (FLayer* Layer : m_LayerStack)
                Layer->OnTick();

            DispatchEvent<FApplicationTickEvent>();
        }

        OnShutdown();

        DispatchEvent<FApplicationShutdownEvent>();
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

void FApplication::PushLayer(FLayer* Layer)
{
    m_LayerStack.PushLayer(Layer);
}

void FApplication::PushOverlay(FLayer* Overlay)
{
    m_LayerStack.PushOverlay(Overlay);
}

void FApplication::PopLayer(FLayer* Layer)
{
    m_LayerStack.PopLayer(Layer);
}

void FApplication::PopOverlay(FLayer* Overlay)
{
    m_LayerStack.PopOverlay(Overlay);
}

void FApplication::ProcessEvents()
{
    FInput::TransitionPressedKeys();
    FInput::TransitionPressedMouseButtons();
    
    m_ApplicationWindow->ProcessEvents();
    
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);

    while (!m_EventQueue.empty())
    {
        auto& EventFunction = m_EventQueue.front();
        EventFunction();

        m_EventQueue.pop();
    }
}

bool FApplication::OnWindowClose()
{
    Close();

    return true;
}

bool FApplication::OnWindowMinimize(const FWindowMinimizeEvent& WindowMinimizeEvent)
{
    bIsWindowMinimized = WindowMinimizeEvent.IsWindowMinimized();

    return true;
}