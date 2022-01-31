
--Hurikan Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir= {}
IncludeDir["glm"] =					"%{wks.location}/Hurikan/vendor/glm"
IncludeDir["Glad"] =					"%{wks.location}/Hurikan/vendor/Glad/include"
IncludeDir["Box2D"] =					"%{wks.location}/Hurikan/vendor/Box2D/include"
IncludeDir["GLFW"] =					"%{wks.location}/Hurikan/vendor/GLFW/include"
IncludeDir["ImGui"] =					"%{wks.location}/Hurikan/vendor/imgui"
IncludeDir["stb_image"] =				"%{wks.location}/Hurikan/vendor/stb_image"
IncludeDir["entt"] =					"%{wks.location}/Hurikan/vendor/entt/include"
IncludeDir["yaml_cpp"] =				"%{wks.location}/Hurikan/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] =				"%{wks.location}/Hurikan/vendor/ImGuizmo"
-- Audio stuff
IncludeDir["HurikanAudio"] =				"%{wks.location}/Hurikan/vendor/HurikanAudio/src"
-- End Audio stuff
IncludeDir["shaderc"] =					"%{wks.location}/Hurikan/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] =				"%{wks.location}/Hurikan/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] =				"%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] =				"%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] =			"%{wks.location}/Hurikan/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] =		"%{wks.location}/Hurikan/vendor/VulkanSDK/Bin"

Library = {}
Library["Vulkan"] =						"%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] =				"%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] =				"%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] =			"%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] =		"%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] =			"%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] =			"%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] =		"%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] =	"%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"