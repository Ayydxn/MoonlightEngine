project "Moonlight Engine"
    targetname "MoonlightEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    targetdir(getProjectBinariesDir("Engine"))
    objdir(getProjectIntermediateDir("Engine"))

    pchsource "%{prj.location}/Source/MoonlightPCH.cpp"
    pchheader "MoonlightPCH.h"

    files
    {
        "%{prj.location}/Source/**.cpp",
        "%{prj.location}/Source/**.h"
    }

    includedirs
    {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Vulkan}",
        "%{IncludeDir.Shaderc}",
        "%{IncludeDir.Glad}",
        
        "%{prj.location}/Source",
        "%{prj.location}/Source/Runtime",
        "%{prj.location}/Source/Runtime/Core"
    }

    links
    {
        "GLFW",
        "Glad"
    }

    defines
    {
        "MOONLIGHT_EXPORT_LIBRARY",
        
        "GLFW_INCLUDE_NONE"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines
        {
            "MOONLIGHT_PLATFORM_WIN64",

            "WIN32_LEAN_AND_MEAN",
            "_WINDLL"
        }

    filter "system:macosx"
        defines
        {
            "MOONLIGHT_PLATFORM_MAC"
        }

    filter "system:linux"
        defines
        {
            "MOONLIGHT_PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines
        {
            "MOONLIGHT_BUILD_DEBUG"
        }

    filter "configurations:Release"
        defines
        {
            "MOONLIGHT_BUILD_RELEASE"
        }

    filter "configurations:Distribution"
        defines
        {
            "MOONLIGHT_BUILD_DISTRIBUTION"
        }
        
    filter { "system:windows", "configurations:Debug" }
        links
        {
            "%{Library.Shaderc_Windows_Debug}"
        }
        
    filter { "system:windows", "configurations:Release or configurations:Distribution" }
        links
        {
            "%{Library.Shaderc_Windows_Release}"
        }