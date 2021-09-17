#include "Hurikan.h"

#include "Hurikan/Core/EntryPoint.h"

#include "GameWindow.h"

class GameApplication : public Hurikan::Application
{
public:
	GameApplication() : Application("BomberFriends", 1280, 720)
	{
		Application::PushLayer(new GameFramework::Game());
	}
};

Hurikan::Application* Hurikan::CreateApplication()
{
	return new GameApplication();
}