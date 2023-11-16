include "Dependencies.lua"

function firstToUpper(str)
    return (str:gsub("^%l", string.upper))
end

function getProjectBinariesDir(projectName)
    return "%{wks.location}/Binaries/" .. projectName .. "/%{cfg.buildcfg}-" .. firstToUpper(os.target()) .. "-%{cfg.architecture}"
end

function getProjectIntermediateDir(projectName)
    return "%{wks.location}/Intermediate/" .. projectName .. "/%{cfg.buildcfg}-" .. firstToUpper(os.target()) .. "-%{cfg.architecture}"
end

workspace "Moonlight Engine"
    architecture "x86_64"
    startproject "Moonlight Editor"
    conformancemode "On"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "system:windows"
	    buildoptions
        {
            "/EHsc",
            "/Zc:preprocessor",
            "/Zc:__cplusplus"
        }

    filter "configurations:Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Distribution"
		optimize "Full"
		symbols "Off"
		
group "Dependencies"
    include "Engine/ThirdParty/GLFW"
    include "Engine/ThirdParty/Glad"
group ""

group "Core"
    include "Engine"
group ""

group "Tools"
    include "Editor"
group ""