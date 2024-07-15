#pragma once

#include "Core/CoreDefines.h"
#include "Core/CoreTypes.h"

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

    void Start();
    void Close();

    static CApplication& GetInstance() { return *m_ApplicationInstance; }

    const CApplicationSpecification& GetSpecification() const { return m_ApplicationSpecification; }
private:
    static CApplication* m_ApplicationInstance;

    CApplicationSpecification m_ApplicationSpecification;

    bool bIsRunning = true;
};

CApplication* CreateApplication(char** Arguments, int32 ArgumentCount);