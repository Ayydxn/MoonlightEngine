-- All Moonlight Engine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Engine/ThirdParty/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/ThirdParty/Glad/include"
IncludeDir["Vulkan"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["Vulkan"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Shaderc_Debug"] = "%{LibraryDir.Vulkan}/shaderc_combinedd.lib"
Library["Shaderc_Release"] = "%{LibraryDir.Vulkan}/shaderc_combined.lib"