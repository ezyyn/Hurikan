#include <Hurikan.h>

#include "GameLayer.h"

#include "Hurikan/Core/EntryPoint.h"

class Storm : public Hurikan::Application
{
public:
	Storm(Hurikan::ApplicationCommandLineArgs args) : Application("Bomber Friends", args, 1600, 900)
	{
		PushLayer(new GameLayer(1600,900));
	}
	~Storm()
	{
	}
};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args) {
	return new Storm(args);
}