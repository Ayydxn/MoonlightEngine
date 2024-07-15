#pragma once

#ifdef MOONLIGHT_EXPORT_LIBRARY
    #ifdef MOONLIGHT_PLATFORM_WIN64
        #define MOONLIGHT_API __declspec(dllexport)
    #else
        #define MOONLIGHT_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef MOONLIGHT_PLATFORM_WIN64
        #define MOONLIGHT_API __declspec(dllimport)
    #else
        #define MOONLIGHT_API
    #endif
#endif
