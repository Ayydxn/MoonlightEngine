﻿-- All dependencies used by Moonlight

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Engine/ThirdParty/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/ThirdParty/GLFW/include"
IncludeDir["Vulkan"] = "%{wks.location}/Engine/ThirdParty/Vulkan/Include"
IncludeDir["Vulkan"] = "%{wks.location}/Engine/ThirdParty/Vulkan/Include"
IncludeDir["Shaderc"] = "%{wks.location}/Engine/ThirdParty/Shaderc/Include"
IncludeDir["Glad"] = "%{wks.location}/Engine/ThirdParty/Glad/include"
IncludeDir["stb_image"] = "%{wks.location}/Engine/ThirdParty/stb_image"
IncludeDir["ImGui"] = "%{wks.location}/Engine/ThirdParty/ImGui"
IncludeDir["glm"] = "%{wks.location}/Engine/ThirdParty/glm"

LibraryDir = {}
LibraryDir["Shaderc_Windows"] = "%{wks.location}/Engine/ThirdParty/Shaderc/Lib/Windows"

Library = {}
Library["Shaderc_Windows_Debug"] = "%{LibraryDir.Shaderc_Windows}/shaderc_sharedd.lib"
Library["Shaderc_Windows_Release"] = "%{LibraryDir.Shaderc_Windows}/shaderc_shared.lib/"