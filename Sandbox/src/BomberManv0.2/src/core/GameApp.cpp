#include <Hurikan.h>

#include <Hurikan/Core/EntryPoint.h>

#include "../core/GameLayer.h"

class GameApp : public Hurikan::Application
{
public:
	GameApp(ApplicationSpecification specification, Hurikan::ApplicationCommandLineArgs args) : Application(specification, args)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new GameLayer());
	}

};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args)
{
	Hurikan::ApplicationSpecification specification;
	specification.Name = "BomberMan v0.2";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.StartMaximized = false;
	specification.VSync = true;
	specification.EnableImGui = false;
	specification.Resizable = false;

	return new GameApp(specification, args);
}