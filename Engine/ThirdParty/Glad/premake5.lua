project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "Off"
    warnings "Off"
       
    targetdir(getProjectBinariesDir("Glad"))
    objdir(getProjectIntermediateDir("Glad"))

	files
	{
        "src/glad.c",
        
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
    }

    includedirs
	{
        "include"
    }
    
	filter "system:windows"
        systemversion "latest"
        
    filter "system:macosx"
        pic "On"
            
    filter "system:linux"
        systemversion "latest"
        pic "On"