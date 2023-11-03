#include "MoonlightPCH.h"
#include "Application.h"

extern bool bIsApplicationRunning;

FApplication::FApplication(const FApplicationSpecification& Specification)
    : m_ApplicationSpecification(Specification), m_CommandLineArguments(Specification.CommandLineArguments)
{
    m_ApplicationInstance = this;

    std::cout << "Welcome to Moonlight Engine!\n";
    std::cout << "Command Line Arguments:\n";

    if (!m_CommandLineArguments.IsEmpty())
    {
        for (const std::string& CommandLineArgument : m_CommandLineArguments)
            std::cout << "  " << CommandLineArgument << "\n";
    }
    else
    {
        std::cout << "  No command line arguments were passed! :(\n";
    }
}

FApplication::~FApplication()
{
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
