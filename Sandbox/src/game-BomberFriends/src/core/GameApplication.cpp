#include <Hurikan.h>

#include "Hurikan/Core/EntryPoint.h"

#include "GameLayer.h"

class GameApplication : public Hurikan::Application
{
public:
	GameApplication(Hurikan::ApplicationCommandLineArgs args) : Hurikan::Application("Bomber Man", args, 1600, 900)
	{
		PushLayer(new GameLayer(1600, 900));
	}
};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args) {
	return new GameApplication(args);
}