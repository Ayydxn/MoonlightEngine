#include "MoonlightPCH.h"
#include "Application.h"
#include "Debug/Profiler.h"
#include "Events/ApplicationEvents.h"
#include "Input/Input.h"
#include "Misc/CommandLineParser.h"
#include "Renderer/Renderer.h"

CApplication::CApplication(const CApplicationSpecification& ApplicationSpecification)
    : m_ApplicationSpecification(ApplicationSpecification), m_CommandLineArguments(ApplicationSpecification.CommandLineArguments)
{
    m_ApplicationInstance = this;
    
    CLogging::Initialize();
    
    ENGINE_LOG_INFO_TAG("Core", "Initializing Moonlight Engine...");

    CRenderer::PreInitialize();

    CWindowSpecification ApplicationWindowSpecification;
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

    if (CCommandLineParser::Param(m_CommandLineArguments, "windowed"))
        ApplicationWindowSpecification.WindowMode = EWindowMode::Windowed;
    
    if (CCommandLineParser::Param(m_CommandLineArguments, "windowedFullscreen"))
        ApplicationWindowSpecification.WindowMode = EWindowMode::WindowedFullscreen;
    
    if (CCommandLineParser::Param(m_CommandLineArguments, "fullscreen"))
        ApplicationWindowSpecification.WindowMode = EWindowMode::Fullscreen;

    if (ApplicationWindowSpecification.WindowMode == EWindowMode::Windowed || ApplicationWindowSpecification.WindowMode == EWindowMode::Fullscreen)
    {
        int32 CmdLineWindowWidth = 0;
        int32 CmdLineWindowHeight = 0;

        if (CCommandLineParser::Value(m_CommandLineArguments, "ResolutionX", &CmdLineWindowWidth))
        {
            if (CmdLineWindowWidth != 0)
                ApplicationWindowSpecification.Width = CmdLineWindowWidth;
        }

        if (CCommandLineParser::Value(m_CommandLineArguments, "ResolutionY", &CmdLineWindowHeight))
        {
            if (CmdLineWindowHeight != 0)
                ApplicationWindowSpecification.Height = CmdLineWindowHeight;
        }
    }

    m_ApplicationWindow = IWindow::Create(ApplicationWindowSpecification);
    m_ApplicationWindow->Initialize();
    m_ApplicationWindow->SetEventCallbackFunction([this](IEvent& Event) { return OnEvent(Event); });

    CInput::Initialize();
    CRenderer::Initialize();

    m_ImGuiLayer = IImGuiLayer::Create();
    PushOverlay(m_ImGuiLayer);
    
    DispatchEvent<CApplicationInitializeEvent>();
}

CApplication::~CApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");

    m_ApplicationWindow->SetEventCallbackFunction(nullptr);

    for (CLayer* Layer : m_LayerStack)
    {
        Layer->OnDetach();
        delete Layer;
    }

    // Clearing the event queue.
    // This also ensures that we free the memory used by it.
    std::queue<std::function<void()>>().swap(m_EventQueue);

    CRenderer::Shutdown();
}

void CApplication::Start()
{
    // TODO: (Ayydxn) Make this configurable but, for now, the engine will always run fixed framerate updates 128 times per second.
    constexpr float TicksPerMilliseconds = 1.0f / 128.0f;
    
    float LastTime = std::chrono::time_point_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now())
        .time_since_epoch()
        .count();
    
    m_Clock.Start();
    m_Clock.Update();

    m_LastFrameTime = m_Clock.GetElapsedTime();
    
    OnInitialize();
    
    while (bIsRunning)
    {
        m_Clock.Update();

        const float ElapsedTime = m_Clock.GetElapsedTime();
        
        m_FrameTime = std::min(ElapsedTime - m_LastFrameTime, 0.0333f);
        m_DeltaTime += m_FrameTime;
        m_LastFrameTime = ElapsedTime;
        
        /*-----------------*/
        /* --  Updating -- */
        /*-----------------*/
        {
            MOONLIGHT_PROFILE_SCOPE("Application Update");

            OnUpdate(m_DeltaTime);

            for (CLayer* Layer : m_LayerStack)
                Layer->OnUpdate(m_DeltaTime);
            
            while (m_DeltaTime >= TicksPerMilliseconds)
            {
                OnFixedUpdate(TicksPerMilliseconds);
            
                for (CLayer* Layer : m_LayerStack)
                    Layer->OnFixedUpdate(TicksPerMilliseconds);

                m_TicksPerSecond++;
                m_DeltaTime -= TicksPerMilliseconds;
            }
        
            ProcessEvents();

            DispatchEvent<CApplicationUpdateEvent>();
        }

        /*-----------------*/
        /* -- Rendering -- */
        /*-----------------*/

        if (!bIsWindowMinimized)
        {
            MOONLIGHT_PROFILE_SCOPE("Application Render");
            
            OnPreRender();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnPreRender();

            CRenderer::BeginFrame();

            OnRender();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnRender();
            
            DispatchEvent<CApplicationRenderEvent>();

            // (Ayydxn) Once rendering has been done, we render ImGui so that it doesn't render under any of the geometry.
            m_ImGuiLayer->BeginFrame();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnImGuiRender();

            m_ImGuiLayer->EndFrame();

            CRenderer::EndFrame();
            
            OnPostRender();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnPostRender();

            m_ApplicationWindow->SwapBuffers();

            m_FramesPerSecond++;
        }

        const auto TimeNow = std::chrono::time_point_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now())
            .time_since_epoch()
            .count();

        if (TimeNow - LastTime >= 1.0f)
        {
            LastTime = TimeNow;
            
            m_FramesPerSecond = 0;
            m_TicksPerSecond = 0;
        }

        MOONLIGHT_PROFILE_MARK_FRAME;
    }

    OnShutdown();

    DispatchEvent<CApplicationShutdownEvent>();
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
    EventDispatcher.Dispatch<CWindowResizeEvent>([this](const CWindowResizeEvent& WindowResizeEvent) { return OnWindowResize(WindowResizeEvent); });

    for (const auto& Layer : m_LayerStack)
    {
        Layer->OnEvent(Event);

        if (Event.bIsHandled)
            break;
    }
}

template<typename Event, typename ... EventArgs>
void CApplication::DispatchEvent(EventArgs&&... Arguments)
{
    //static_assert(std::is_assignable_v<IEvent, Event>);

    std::shared_ptr<Event> EventInstance = std::make_shared<Event>(std::forward<EventArgs>(Arguments)...);
    OnEvent(*EventInstance);
}

template <typename EventFunction>
void CApplication::QueueEvent(EventFunction&& EventFunc)
{
    std::scoped_lock<std::mutex> Lock(m_EventQueueMutex);
    m_EventQueue.push(EventFunc);
}

void CApplication::PushLayer(CLayer* Layer)
{
    m_LayerStack.PushLayer(Layer);
}

void CApplication::PushOverlay(CLayer* Overlay)
{
    m_LayerStack.PushOverlay(Overlay);
}

void CApplication::PopLayer(CLayer* Layer)
{
    m_LayerStack.PopLayer(Layer);
}

void CApplication::PopOverlay(CLayer* Overlay)
{
    m_LayerStack.PopOverlay(Overlay);
}

void CApplication::ProcessEvents()
{
    CInput::TransitionPressedKeys();
    CInput::TransitionPressedMouseButtons();
    
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
    bIsWindowMinimized = WindowMinimizeEvent.IsWindowMinimized();

    return true;
}

bool CApplication::OnWindowResize(const CWindowResizeEvent& WindowResizeEvent)
{
    CRenderer::GetContext()->OnWindowResize(WindowResizeEvent.GetWidth(), WindowResizeEvent.GetHeight());

    return true;
}
