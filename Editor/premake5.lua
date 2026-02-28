project "Moonlight Editor"
    targetname "MoonlightEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"
    fastuptodate "Off"

    targetdir(getProjectBinariesDir("Editor"))
    objdir(getProjectIntermediateDir("Editor"))

    files
    {
        "%{prj.location}/Source/**.cpp",
        "%{prj.location}/Source/**.h",
        
        "%{prj.location}/Resources/**/**.**"
    }

    IncludeDependencies({ "spdlog", "glm", "ImGui", "Slang" })

    includedirs
    {
        "%{prj.location}/Source",

        "%{wks.location}/Engine/Source",
        "%{wks.location}/Engine/Source/Runtime",
        "%{wks.location}/Engine/Source/Runtime/Core"
    }

    links
    {
        "Moonlight Engine"
    }

    filter "system:windows"
        systemversion "latest"
        entrypoint "WinMainCRTStartup"

        defines
        {
            "MOONLIGHT_PLATFORM_WIN64"
        }

        postbuildcommands
        {
            '{COPYFILE} "%{getProjectBinariesDir("Engine")}/*.dll" "%{cfg.buildtarget.directory}"',
            '{COPYFILE} "%{getProjectBinariesDir("GLFW")}/*.dll" "%{cfg.buildtarget.directory}"'
        }

    filter "system:macosx"
        defines
        {
            "MOONLIGHT_PLATFORM_MAC"
        }

    filter "system:linux"
        files { "%{getProjectBinariesDir(\"Engine\")}/*.so" }
        buildaction "Copy"

        files { "%{getProjectBinariesDir(\"GLFW\")}/*.so" }
        buildaction "Copy"

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
        kind "WindowedApp"

        defines
        {
            "MOONLIGHT_BUILD_DISTRIBUTION"
        }
