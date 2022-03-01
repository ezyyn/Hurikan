#include "BomberMan/Core/GameManager.hpp"

#include <Hurikan/Core/EntryPoint.h>

class GApplication : public Hurikan::Application
{
public:
	GApplication(const Hurikan::ApplicationSpecification& specification, Hurikan::ApplicationCommandLineArgs args) : Application(specification, args)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new GameManager);
	}

};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args)
{
	Hurikan::ApplicationSpecification specification;
	specification.Name = "BomberMan v0.3";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.StartMaximized = false;
	specification.VSync = true;
#ifdef HU_DEBUG
	specification.EnableImGui = true;
#else
	specification.EnableImGui = false;
#endif
	specification.Resizable = false;

	return new GApplication(specification, args);
}