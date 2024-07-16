#pragma once

#include "Window.h"
#include "Core/CoreDefines.h"
#include "Core/CoreTypes.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"

#include <mutex>
#include <queue>
#include <string>

struct CApplicationSpecification
{
    std::string Name = "Moonlight Engine";
    uint32 WindowWidth = 1600;
    uint32 WindowHeight = 900;
    EWindowMode WindowMode = EWindowMode::Windowed;
    bool bEnableVSync = true;
    bool bEnableWindowDecoration = true;
    bool bEnableWindowResizing = true;
};

class MOONLIGHT_API CApplication
{
public:
    CApplication(CApplicationSpecification ApplicationSpecification = CApplicationSpecification());
    virtual ~CApplication();

    virtual void OnEvent(IEvent& Event);
    
    void Start();
    void Close();

    template<typename Event, typename ... EventArgs>
    void DispatchEvent(EventArgs&& ... Arguments);

    template<typename EventFunction>
    void QueueEvent(EventFunction&& EventFunc);

    static CApplication& GetInstance() { return *m_ApplicationInstance; }

    const IWindow& GetWindow() const { return *m_ApplicationWindow; }
    
    const CApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    void ProcessEvents();

    bool OnWindowClose();
    bool OnWindowMinimized(const CWindowMinimizeEvent& WindowMinimizeEvent);
private:
    static CApplication* m_ApplicationInstance;

    std::shared_ptr<IWindow> m_ApplicationWindow;

    CApplicationSpecification m_ApplicationSpecification;

    std::mutex m_EventQueueMutex;
    std::queue<std::function<void()>> m_EventQueue;

    bool bIsWindowMinizmied = false;
    bool bIsRunning = true;
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount);