
--Sandbox

project "Sandbox"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	floatingpoint "Fast"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	includedirs 
	{
		"src",
		"%{IncludeDir.Box2D}",
		"%{wks.location}/Hurikan/vendor/spdlog/include",
		"%{wks.location}/Hurikan/src",
		"%{wks.location}/Hurikan/vendor/imgui",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.HurikanAudio}",
	}
	links 
	{
		"HurikanAudio",
		"Box2D",
		"Hurikan"
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"HU_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			kind "ConsoleApp"
			defines "HU_DEBUG"
			runtime "Debug"
			symbols "on"

			postbuildcommands
			{
				"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
			}

		filter "configurations:Release"
			kind "WindowedApp"
			defines "HU_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			kind "WindowedApp"
			defines "HU_DIST"
			runtime "Release"
			optimize "on"