#include "Hurikan.h"

#include "GameWindow.h"

#include "Hurikan/Core/EntryPoint.h"

class GameApplication : public Hurikan::Application
{
public:
	GameApplication(ApplicationCommandLineArgs args) : Application("BomberFriends", args, 1280, 720)
	{
		Application::PushLayer(new GameFramework::GameLayer());
	}
};

Hurikan::Application* Hurikan::CreateApplication(ApplicationCommandLineArgs args)
{
	return new GameApplication(args);
}