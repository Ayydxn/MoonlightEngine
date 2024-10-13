#include "MoonlightPCH.h"
#include "Application.h"
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
    
    DispatchEvent<CApplicationInitializeEvent>();

    /*-----------------------------------------*/
    /* -- TEMP: REMOVE ASAP - Triangle Test -- */
    /*-----------------------------------------*/

    constexpr float Vertices[9] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    constexpr uint32 Indices[] = { 0, 1, 2 };

    const CVertexBufferLayout VertexBufferLayout =
    {
        { "Positions", EShaderDataType::Float3 }
    };

    CGraphicsPipelineSpecification GraphicsPipelineSpecification;
    GraphicsPipelineSpecification.VertexBufferLayout = VertexBufferLayout;

    m_Shader = CRenderer::GetShaderLibrary()->GetShader("DefaultShader");

    m_VertexBuffer = IVertexBuffer::Create(Vertices, sizeof(Vertices));
    m_IndexBuffer = IIndexBuffer::Create(Indices, sizeof(Indices));
    m_GraphicsPipeline = IGraphicsPipeline::Create(GraphicsPipelineSpecification);
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
    OnInitialize();
    
    while (bIsRunning)
    {
        /*-----------------*/
        /* --  Updating -- */
        /*-----------------*/
        
        OnUpdate();

        for (CLayer* Layer : m_LayerStack)
            Layer->OnUpdate();
        
        ProcessEvents();

        DispatchEvent<CApplicationUpdateEvent>();

        /*-----------------*/
        /* -- Rendering -- */
        /*-----------------*/

        if (!bIsWindowMinizmied)
        {
            OnPreRender();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnPreRender();

            CRenderer::BeginFrame();

            OnRender();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnRender();

            /* -- TEMP: REMOVE ASAP - Triangle Test -- */
            CRenderer::DrawIndexed({ m_Shader, m_VertexBuffer, m_IndexBuffer, m_GraphicsPipeline });
            
            DispatchEvent<CApplicationRenderEvent>();

            CRenderer::EndFrame();
            
            OnPostRender();

            for (CLayer* Layer : m_LayerStack)
                Layer->OnPostRender();

            m_ApplicationWindow->SwapBuffers();
        }
        
        /*---------------*/
        /* -- Ticking -- */
        /*---------------*/

        OnTick();

        for (CLayer* Layer : m_LayerStack)
            Layer->OnTick();

        DispatchEvent<CApplicationTickEvent>();
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
    bIsWindowMinizmied = WindowMinimizeEvent.IsWindowMinimized();

    return true;
}

bool CApplication::OnWindowResize(const CWindowResizeEvent& WindowResizeEvent)
{
    CRenderer::GetContext()->OnWindowResize(WindowResizeEvent.GetWidth(), WindowResizeEvent.GetHeight());\

    return true;
}
