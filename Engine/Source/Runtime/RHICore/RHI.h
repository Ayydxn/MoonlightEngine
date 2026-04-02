#pragma once

#include "RHIFactory.h"

class CRHI
{
public:
    static void Initialize();
    static void Shutdown();
    
    static IRHIFactory& GetFactory();
private:
    static std::unique_ptr<IRHIFactory> m_Factory;
};
