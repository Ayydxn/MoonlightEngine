#pragma once

#include "Application.h"

extern CApplication* CreateApplication(char** Arguments, int32 ArgumentCount);

inline int32 MoonlightMain(char** Arguments, int32 ArgumentCount)
{
    CApplication* Application = CreateApplication(Arguments, ArgumentCount);
    Application->Start();
    delete Application;

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