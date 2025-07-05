include "./Scripts/Lua/PremakeUtils.lua"
include "./Dependencies.lua"

function firstToUpper(str)
    return (str:gsub("^%l", string.upper))
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

    defines
    {
        "TRACY_ENABLE",
        "TRACY_ON_DEMAND"
    }

    filter "system:windows"
	    buildoptions
        {
            "/EHsc",
            "/Zc:preprocessor",
            "/Zc:__cplusplus"
        }
    
        disablewarnings
        {
            "4251"
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
    include "Engine/ThirdParty/tracy"
group ""

group "Core"
    include "Engine"
group ""

group "Tools"
    include "Editor"
group ""