#pragma once

#include "Window.h"
#include "Core/CoreDefines.h"
#include "Core/CoreTypes.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "ImGui/ImGuiLayer.h"
#include "Layers/LayerStack.h"
#include "Misc/CommandLineArguments.h"

#include "RHICore/Shader.h"
#include "RHICore/Texture.h"

#include <mutex>
#include <queue>
#include <string>

class IVertexBuffer;
class IIndexBuffer;
class IGraphicsPipeline;

struct CApplicationSpecification
{
    std::string Name = "Moonlight Engine";
    uint32 WindowWidth = 1600;
    uint32 WindowHeight = 900;
    EWindowMode WindowMode = EWindowMode::Windowed;
    bool bEnableVSync = true;
    bool bEnableWindowDecoration = true;
    bool bEnableWindowResizing = true;

    CCommandLineArguments CommandLineArguments;
};

class MOONLIGHT_API CApplication
{
public:
    CApplication(const CApplicationSpecification& ApplicationSpecification = CApplicationSpecification());
    virtual ~CApplication();

    virtual void OnInitialize() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(IEvent& Event);
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnPostRender() {}
    virtual void OnTick() {}
    virtual void OnShutdown() {}
    
    void Start();
    void Close();

    template<typename Event, typename ... EventArgs>
    void DispatchEvent(EventArgs&& ... Arguments);

    template<typename EventFunction>
    void QueueEvent(EventFunction&& EventFunc);

    void PushLayer(CLayer* Layer);
    void PushOverlay(CLayer* Overlay);
    void PopLayer(CLayer* Layer);
    void PopOverlay(CLayer* Overlay);
    
    static CApplication& GetInstance() { return *m_ApplicationInstance; }

    IWindow& GetWindow() const { return *m_ApplicationWindow; }
    
    const CApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    void ProcessEvents();

    bool OnWindowClose();
    bool OnWindowMinimized(const CWindowMinimizeEvent& WindowMinimizeEvent);
    bool OnWindowResize(const CWindowResizeEvent& WindowResizeEvent);
private:
    inline static CApplication* m_ApplicationInstance = nullptr;
    
    std::shared_ptr<IWindow> m_ApplicationWindow;

    IImGuiLayer* m_ImGuiLayer;

    CApplicationSpecification m_ApplicationSpecification;
    CCommandLineArguments m_CommandLineArguments;
    CLayerStack m_LayerStack;
    
    std::mutex m_EventQueueMutex;
    std::queue<std::function<void()>> m_EventQueue;

    bool bIsWindowMinizmied = false;
    bool bIsRunning = true;

    std::shared_ptr<IVertexBuffer> m_VertexBuffer;
    std::shared_ptr<IIndexBuffer> m_IndexBuffer;
    std::shared_ptr<IGraphicsPipeline> m_GraphicsPipeline;
    std::shared_ptr<IShader> m_Shader;
    std::shared_ptr<ITexture> m_PlaceholderTexture;
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount);