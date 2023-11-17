project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "Off"
    
    targetdir(getProjectBinariesDir("Glad"))
    objdir(getProjectIntermediateDir("Glad"))

    files
    {
        "src/glad.c",

        "include/glad/glad.h",
        "include/KHR/khrplatform.h"
    }

    includedirs
    {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "Speed"

    filter "configurations:Distribution"
        runtime "Release"
        symbols "Off"
        optimize "Speed"