#pragma once
#include "hupch.h"

#ifdef HU_PLATFORM_WINDOWS

extern Hurikan::Application* Hurikan::CreateApplication();

int main(int argc, char** argv) {
	Hurikan::Log::Init();

	HU_PROFILE_BEGIN_SESSION("Startup", "HurikanProfile-Startup.json");
	auto app = Hurikan::CreateApplication();
	HU_PROFILE_END_SESSION();
	HU_PROFILE_BEGIN_SESSION("Runtime", "HurikanProfile-Runtime.json");
	app->Run();
	HU_PROFILE_END_SESSION();

	HU_PROFILE_BEGIN_SESSION("Shutdown", "HurikanProfile-Shutdown.json");
	delete app;
	HU_PROFILE_END_SESSION();
}

#endif
