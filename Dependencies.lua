-- All dependencies used by Moonlight

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Engine/ThirdParty/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/ThirdParty/GLFW/include"
IncludeDir["Vulkan"] = "%{wks.location}/Engine/ThirdParty/Vulkan/Include"
IncludeDir["Vulkan"] = "%{wks.location}/Engine/ThirdParty/Vulkan/Include"
IncludeDir["Shaderc"] = "%{wks.location}/Engine/ThirdParty/Shaderc/Include"
IncludeDir["Glad"] = "%{wks.location}/Engine/ThirdParty/Glad/include"

LibraryDir = {}
LibraryDir["Shaderc_Windows"] = "%{wks.location}/Engine/ThirdParty/Shaderc/Lib/Windows"

Library = {}
Library["Shaderc_Windows_Debug"] = "%{LibraryDir.Shaderc_Windows}/shaderc_sharedd.lib"
Library["Shaderc_Windows_Release"] = "%{LibraryDir.Shaderc_Windows}/shaderc_shared.lib/"