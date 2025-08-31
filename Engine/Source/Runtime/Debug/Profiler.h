#pragma once

#define MOONLIGHT_ENABLE_PROFILING !MOONLIGHT_BUILD_DISTRIBUTION

#if MOONLIGHT_ENABLE_PROFILING
    #include <tracy/Tracy.hpp>
#endif

#if MOONLIGHT_ENABLE_PROFILING
    #define MOONLIGHT_PROFILE_MARK_FRAME FrameMark

    #define MOONLIGHT_PROFILE_FUNCTION(...) ZoneScoped##__VA_OPT__(N(__VA_ARGS__))
    #define MOONLIGHT_PROFILE_SCOPE(...) MOONLIGHT_PROFILE_FUNCTION(__VA_ARGS__)
    #define MOONLIGHT_PROFILE_SCOPE_DYNAMIC(Name) ZoneScoped; ZoneName(Name, strlen(Name))
    #define MOONLIGHT_PROFILE_THREAD(...) tracy::SetThreadName(__VA_ARGS__)

    #define MOONLIGHT_PROFILE_GPU(...) TracyGpuZone(__VA_ARGS__)
#else
    #define MOONLIGHT_PROFILE_MARK_FRAME

    #define MOONLIGHT_PROFILE_FUNCTION(...)
    #define MOONLIGHT_PROFILE_SCOPE(...)
    #define MOONLIGHT_PROFILE_SCOPE_DYNAMIC(Name)
    #define MOONLIGHT_PROFILE_THREAD(...)

    #define MOONLIGHT_PROFILE_GPU(...)
#endif