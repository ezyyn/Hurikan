#pragma once
#include "hupch.h"

#include "Hurikan/Core/Base.h"
#include "Hurikan/Core/Application.h"

#ifdef HU_PLATFORM_WINDOWS

#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

extern Hurikan::Application* Hurikan::CreateApplication(ApplicationCommandLineArgs args);

#define MEMTRACK
#ifdef MEMTRACK

extern size_t usage = 0;

void* operator new(size_t size)
{
	usage += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	usage -= size;
	free(memory);
}
#endif

int main(int argc, char** argv) 
{
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

	Hurikan::Log::Init();

	HU_PROFILE_BEGIN_SESSION("Startup", "HurikanProfile-Startup.json");
	auto app = Hurikan::CreateApplication({argc, argv});
	HU_PROFILE_END_SESSION();
	
	HU_PROFILE_BEGIN_SESSION("Runtime", "HurikanProfile-Runtime.json");
	app->Run();
	HU_PROFILE_END_SESSION();
	
	HU_PROFILE_BEGIN_SESSION("Shutdown", "HurikanProfile-Shutdown.json");
	delete app;

	HU_PROFILE_END_SESSION();

	_CrtDumpMemoryLeaks();

#ifdef MEMTRACK
	HU_CORE_WARN("Memory leak: {0}", usage);
#endif
	return 0;
}

#endif
