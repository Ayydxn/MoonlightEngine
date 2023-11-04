#include "MoonlightPCH.h"
#include "Application.h"

extern bool bIsApplicationRunning;

FApplication::FApplication(const FApplicationSpecification& Specification)
    : m_ApplicationSpecification(Specification), m_CommandLineArguments(Specification.CommandLineArguments)
{
    m_ApplicationInstance = this;

    FLogging::Initialize();

    ENGINE_LOG_INFO_TAG("Core", "Initializing Moonlight Engine...");
}

FApplication::~FApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");
}

void FApplication::Start()
{
    while (bIsRunning)
    {
        
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
