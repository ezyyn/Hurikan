
--SOLUTION WORKSPACE

include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Hurikan"
	architecture "x86_64"
	startproject "Storm"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

group "Dependencies"
	include "vendor/premake"
	include "Hurikan/vendor/GLFW"
	include "Hurikan/vendor/Glad"
	include "Hurikan/vendor/ImGui"
	include "Hurikan/vendor/yaml-cpp"
group ""

include "Hurikan"
include "Sandbox"
include "Storm"
