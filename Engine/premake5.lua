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
        "%{prj.location}/Source/**.h",

        "%{prj.location}/ThirdParty/stb_image/**.cpp",
        "%{prj.location}/ThirdParty/stb_image/**.h",

        "%{prj.location}/ThirdParty/ImGui/*.cpp",
        "%{prj.location}/ThirdParty/ImGui/*.h",
        "%{prj.location}/ThirdParty/ImGui/backends/imgui_impl_glfw.cpp",
        "%{prj.location}/ThirdParty/ImGui/backends/imgui_impl_glfw.h",
        "%{prj.location}/ThirdParty/ImGui/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/ThirdParty/ImGui/backends/imgui_impl_opengl3.h",
        "%{prj.location}/ThirdParty/ImGui/backends/imgui_impl_opengl3_loader.h",
        
        "%{prj.location}/ThirdParty/tracy/public/TracyClient.cpp"
    }

    IncludeDependencies({ "spdlog", "GLFW", "Vulkan", "Shaderc", "Glad", "stb_image", "ImGui", "glm", "tracy" })

    includedirs
    {
        "%{prj.location}/Source",
        "%{prj.location}/Source/Runtime",
        "%{prj.location}/Source/Runtime/Core"
    }

    defines
    {
        "MOONLIGHT_EXPORT_LIBRARY",

        "GLFW_INCLUDE_NONE"
    }

    filter "files:ThirdParty/ImGui/**.cpp or files:ThirdParty/tracy/public/TracyClient.cpp"
        flags { "NoPCH" }

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

        excludes
        {
            "**/Windows/**"
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
