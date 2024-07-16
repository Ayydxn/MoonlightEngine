#pragma once

#include "Core/CoreDefines.h"
#include "Core/CoreTypes.h"
#include "Events/Event.h"

#include <mutex>
#include <queue>
#include <string>

struct CApplicationSpecification
{
    std::string Name = "Moonlight Engine";
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

    const CApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    void ProcessEvents();
private:
    static CApplication* m_ApplicationInstance;

    CApplicationSpecification m_ApplicationSpecification;

    std::mutex m_EventQueueMutex;
    std::queue<std::function<void()>> m_EventQueue;
    
    bool bIsRunning = true;
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount);