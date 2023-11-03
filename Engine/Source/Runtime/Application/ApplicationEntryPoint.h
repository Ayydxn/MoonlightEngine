#pragma once

#include "Application.h"

extern FApplication* CreateApplication(char** Arguments, int32 ArgumentCount);

inline bool bIsApplicationRunning = true;

inline int32 MoonlightMain(char** Arguments, int32 ArgumentCount)
{
    // We encapsulate the application in a double while loop (this one and the one present in FApplication::Start) so that restarting programatically is possible. 
    while (bIsApplicationRunning)
    {
        FApplication* Application = CreateApplication(Arguments, ArgumentCount);
        Application->Start();
        delete Application;
    }

    return 0;
}

#ifdef MOONLIGHT_PLATFORM_WIN64

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, PSTR lpCommandLine, int nCommandShow)
{
    return MoonlightMain(__argv, __argc);
}

#else

int main(int ArgumentCount, char** Arguments)
{
    return MoonlightMain(Arguments, ArgumentCount);
}
    
#endif