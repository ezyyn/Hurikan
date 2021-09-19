#include "Hurikan.h"

#include "GameWindow.h"

#include "Hurikan/Core/EntryPoint.h"

class GameApplication : public Hurikan::Application
{
public:
	GameApplication() : Application("BomberFriends", 1280, 720)
	{
		Application::PushLayer(new GameFramework::GameLayer());
	}
};

Hurikan::Application* Hurikan::CreateApplication()
{
	return new GameApplication();
}