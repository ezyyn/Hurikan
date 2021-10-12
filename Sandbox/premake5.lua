
--Sandbox

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	includedirs 
	{
		"%{wks.location}/Hurikan/vendor/spdlog/include",
		"%{wks.location}/Hurikan/src",
		"%{wks.location}/Hurikan/vendor/imgui",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
	}
	links 
	{
		"Hurikan"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"HU_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "HU_DEBUG"
			runtime "Debug"
			symbols "on"

			postbuildcommands
			{
				"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
			}

		filter "configurations:Release"
			defines "HU_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "HU_DIST"
			runtime "Release"
			optimize "on"