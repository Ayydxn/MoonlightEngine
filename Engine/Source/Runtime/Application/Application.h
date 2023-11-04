#pragma once

#include "Core/Misc/CommandLineArguments.h"
#include "Events/Event.h"

#include <mutex>
#include <queue>
#include <string>

struct FApplicationSpecification
{
    std::string Name = "Moonlight Engine";

    FCommandLineArguments CommandLineArguments;
};

class FApplication
{
    using EventCallbackFunction = std::function<void(FEvent&)>;
public:
    FApplication(const FApplicationSpecification& Specification = FApplicationSpecification());
    virtual ~FApplication();

    virtual void OnEvent(FEvent& Event);

    void Start();
    void Restart();
    void Close();

    template<typename Event, typename ... EventArgs>
    void DispatchEvent(EventArgs&& ... Arguments);

    template<typename EventFunction>
    void QueueEvent(EventFunction&& EventFunc);
    
    static FApplication& GetInstance() { return *m_ApplicationInstance; }

    const FCommandLineArguments& GetCommandLineArguments() const { return m_CommandLineArguments; }

    const FApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    void ProcessEvents();
private:
    inline static FApplication* m_ApplicationInstance = nullptr;

    FApplicationSpecification m_ApplicationSpecification;
    FCommandLineArguments m_CommandLineArguments;

    std::mutex m_EventQueueMutex;
    std::queue<std::function<void()>> m_EventQueue;

    bool bIsRunning = true;
};

FApplication* CreateApplication(char** Arguments, int32 ArgumentCount);
