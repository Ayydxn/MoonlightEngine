#pragma once

#include "Core/Misc/CommandLineArguments.h"

#include <string>

struct FApplicationSpecification
{
    std::string Name = "Moonlight Engine";

    FCommandLineArguments CommandLineArguments;
};

class FApplication
{
public:
    FApplication(const FApplicationSpecification& Specification = FApplicationSpecification());
    virtual ~FApplication();

    void Start();
    void Restart();
    void Close();
    
    static FApplication& GetInstance() { return *m_ApplicationInstance; }

    const FCommandLineArguments& GetCommandLineArguments() const { return m_CommandLineArguments; }

    const FApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    inline static FApplication* m_ApplicationInstance = nullptr;

    FApplicationSpecification m_ApplicationSpecification;
    FCommandLineArguments m_CommandLineArguments;
    
    bool bIsRunning = true;
};

FApplication* CreateApplication(char** Arguments, int32 ArgumentCount);