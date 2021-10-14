#include <Hurikan.h>

#include "GameLayer.h"

#include "Hurikan/Core/EntryPoint.h"

class Storm : public Hurikan::Application
{
public:
	Storm(Hurikan::ApplicationCommandLineArgs args) : Application("Bomber Friends", args, 1280, 720)
	{
		PushLayer(new GameLayer(1280, 720));
	}
	~Storm()
	{
	}
};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args) {
	return new Storm(args);
}