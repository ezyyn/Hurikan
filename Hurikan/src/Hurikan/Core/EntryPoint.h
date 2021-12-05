#pragma once
#include "hupch.h"

#include "Hurikan/Core/Base.h"
#include "Hurikan/Core/Application.h"

#ifdef HU_PLATFORM_WINDOWS

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

int main(int argc, char** argv) {
	Hurikan::Log::Init();

	HU_PROFILE_BEGIN_SESSION("Startup", "HurikanProfile-Startup.json");
	auto app = Hurikan::CreateApplication({argc, argv});
	HU_PROFILE_END_SESSION();
	HU_PROFILE_BEGIN_SESSION("Runtime", "HurikanProfile-Runtime.json");
	app->Run();
	HU_PROFILE_END_SESSION();

	HU_PROFILE_BEGIN_SESSION("Shutdown", "HurikanProfile-Shutdown.json");
	delete app;

	HU_CORE_WARN("Memory leak: {0}",usage)

	HU_PROFILE_END_SESSION();
}

#endif
