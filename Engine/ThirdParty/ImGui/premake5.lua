project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
    staticruntime "off"

	targetdir (getProjectBinariesDir("ImGui"))
	objdir (getProjectIntermediateDir("ImGui"))

	files
	{
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
		"imconfig.h",
		
		"backends/imgui_impl_glfw.cpp",
		"backends/imgui_impl_glfw.h",
		"backends/imgui_impl_opengl3.cpp",
		"backends/imgui_impl_opengl3.h",
		"backends/imgui_impl_opengl3_loader.h"
	}

    includedirs
    {
        "%{IncludeDir.GLFW}",
        
        "%{prj.location}"
    }

	filter "system:windows"
		systemversion "latest"
		
		files
		{
            "backends/imgui_impl_dx11.cpp",
            "backends/imgui_impl_dx11.h",
            "backends/imgui_impl_dx12.cpp",
            "backends/imgui_impl_dx12.h"
        }
    
    filter "system:macosx"
        files
        {
            "backends/imgui_impl_metal.mm",
            "backends/imgui_impl_osx.mm",
            "backends/imgui_impl_osx.h",
        }

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

    filter "configurations:Distribution"
		runtime "Release"
		optimize "On"
        symbols "Off"