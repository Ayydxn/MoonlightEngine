#include "MoonlightPCH.h"
#include "Application.h"

CApplication* CApplication::m_ApplicationInstance = nullptr;

CApplication::CApplication(const CApplicationSpecification& ApplicationSpecification)
{
    m_ApplicationInstance = this;
    
    printf("Hello from %s! (Powered by Moonlight Engine)", ApplicationSpecification.Name.c_str());
}

CApplication::~CApplication()
{
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
