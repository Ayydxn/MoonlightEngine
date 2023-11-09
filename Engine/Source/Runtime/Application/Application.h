#pragma once

#include "Window.h"
#include "Core/Misc/CommandLineArguments.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Layers/LayerStack.h"

#include <mutex>
#include <queue>
#include <string>

struct FApplicationSpecification
{
    std::string Name = "Moonlight Engine";
    uint32 WindowWidth = 1600;
    uint32 WindowHeight = 900;
    EWindowMode WindowMode = EWindowMode::Windowed;
    bool bEnableVSync = true;
    bool bEnableWindowDecoration = false;
    bool bEnableWindowResizing = true;
    
    FCommandLineArguments CommandLineArguments;
};

class FApplication
{
    using EventCallbackFunction = std::function<void(FEvent&)>;
public:
    FApplication(const FApplicationSpecification& Specification = FApplicationSpecification());
    virtual ~FApplication();

    virtual void OnInitialize() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(FEvent& Event);
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnPostRender() {}
    virtual void OnTick() {}
    virtual void OnShutdown() {}

    void Start();
    void Restart();
    void Close();

    template<typename Event, typename ... EventArgs>
    void DispatchEvent(EventArgs&& ... Arguments);

    template<typename EventFunction>
    void QueueEvent(EventFunction&& EventFunc);

    void PushLayer(FLayer* Layer);
    void PushOverlay(FLayer* Overlay);
    void PopLayer(FLayer* Layer);
    void PopOverlay(FLayer* Overlay);
    
    static FApplication& GetInstance() { return *m_ApplicationInstance; }

    const FCommandLineArguments& GetCommandLineArguments() const { return m_CommandLineArguments; }

    const FApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    void ProcessEvents();

    bool OnWindowClose();
    bool OnWindowMinimize(const FWindowMinimizeEvent& WindowMinimizeEvent);
private:
    inline static FApplication* m_ApplicationInstance = nullptr;

    std::shared_ptr<FWindow> m_ApplicationWindow = nullptr;

    FApplicationSpecification m_ApplicationSpecification;
    FCommandLineArguments m_CommandLineArguments;
    FLayerStack m_LayerStack;

    std::mutex m_EventQueueMutex;
    std::queue<std::function<void()>> m_EventQueue;

    bool bIsWindowMinimized = false;
    bool bIsRunning = true;
};

FApplication* CreateApplication(char** Arguments, int32 ArgumentCount);
