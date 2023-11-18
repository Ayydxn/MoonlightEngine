project "Moonlight Engine"
    targetname "MoonlightEngine"
    kind "StaticLib"
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
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Vulkan}",
        
        "%{prj.location}/Source",
        "%{prj.location}/Source/Runtime",
        "%{prj.location}/Source/Runtime/Core"
    }
    
    libdirs
    {
        "%{LibraryDir.Vulkan}"
    }
    
    links
    {
        "GLFW",
        "Glad",
        
        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "MOONLIGHT_PLATFORM_WIN64",

            "WIN32_LEAN_AND_MEAN"
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
        links
        {
            "%{Library.Shaderc_Debug}"
        }
        
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
        
    filter { "configurations:Release or Distribution" }
        links
        {
            "%{Library.Shaderc_Release}"
        }