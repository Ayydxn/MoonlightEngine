#include "MoonlightPCH.h"
#include "Application.h"

CApplication* CApplication::m_ApplicationInstance = nullptr;

CApplication::CApplication(CApplicationSpecification ApplicationSpecification)
    : m_ApplicationSpecification(std::move(ApplicationSpecification))
{
    m_ApplicationInstance = this;
    
    CLogging::Initialize();
    
    ENGINE_LOG_INFO_TAG("Core", "Initializing Moonlight Engine...");
}

CApplication::~CApplication()
{
    ENGINE_LOG_INFO_TAG("Core", "Shutting down...");
}

void CApplication::Start()
{
    while (bIsRunning)
    {
        
    }
}

void CApplication::Close()
{
    bIsRunning = false;
}
