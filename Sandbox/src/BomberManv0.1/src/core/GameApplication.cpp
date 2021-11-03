#include <Hurikan.h>
using namespace Hurikan;

#include "Hurikan/Core/EntryPoint.h"

#include "GameLayer.h"

class GameApplication : public Hurikan::Application
{
public:
	GameApplication(ApplicationCommandLineArgs args) : Application("Bomber Man", args, 1600, 900)
	{
		PushLayer(new GameLayer(1600, 900));
	}
private:

};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args) {
	return new GameApplication(args);
}