#include "GameApplication.h"

#include <Hurikan/Core/EntryPoint.h>

#include "../core/GameLayer.h"

GameApplication::GameApplication(Hurikan::ApplicationCommandLineArgs args) : Hurikan::Application("Bomber Man", args, 1600, 900)
{
	m_GameLayer = new GameLayer(this);
	PushLayer(m_GameLayer);
}

Hurikan::Application* Hurikan::CreateApplication(ApplicationCommandLineArgs args)
{
	return new GameApplication(args);
}
