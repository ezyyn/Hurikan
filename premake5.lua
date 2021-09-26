--SOLUTION WORKSPACE

workspace "Hurikan"
	architecture "x64"
	startproject "Storm"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

IncludeDir= {}
IncludeDir["glm"] = "Hurikan/vendor/glm"
IncludeDir["Glad"] = "Hurikan/vendor/Glad/include"
IncludeDir["GLFW"] = "Hurikan/vendor/GLFW/include"
IncludeDir["ImGui"] = "Hurikan/vendor/imgui"
IncludeDir["stb_image"] = "Hurikan/vendor/stb_image"
IncludeDir["entt"] = "Hurikan/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Hurikan/vendor/yaml-cpp/include"

group "Dependencies"
	include "Hurikan/vendor/Glad"
	include "Hurikan/vendor/GLFW"
	include "Hurikan/vendor/ImGui"
	include "Hurikan/vendor/yaml-cpp"
group ""

project "Hurikan"
	location "Hurikan"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hupch.h"
	pchsource "Hurikan/src/hupch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}
	links {
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HU_PLATFORM_WINDOWS",
			"HU_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		filter "configurations:Debug"
			defines "HU_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "HU_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "HU_DIST"
			runtime "Release"
			optimize "on"
			
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs {
		"Hurikan/vendor/spdlog/include",
		"Hurikan/src",
		"Hurikan/vendor/imgui",
		"%{IncludeDir.glm}",
		"Hurikan/vendor/Glad/include",
		"%{IncludeDir.entt}"
	}
	links {
		"Hurikan"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HU_PLATFORM_WINDOWS"
		}
		filter "configurations:Debug"
			defines "HU_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "HU_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "HU_DIST"
			runtime "Release"
			optimize "on"
--HURIKAN EDITOR
project "Storm"
	location "Storm"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs {
		"Hurikan/vendor/spdlog/include",
		"Hurikan/src",
		"Hurikan/vendor/imgui",
		"%{IncludeDir.glm}",
		"Hurikan/vendor/Glad/include",
		"%{IncludeDir.entt}"
	}
	links {
		"Hurikan"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HU_PLATFORM_WINDOWS"
		}
		filter "configurations:Debug"
			defines "HU_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "HU_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "HU_DIST"
			runtime "Release"
			optimize "on"
			