#pragma once
#include <Hurikan.h>

class GameApplication : public Hurikan::Application
{
public:
	GameApplication(Hurikan::ApplicationCommandLineArgs args);
private:
	Hurikan::Layer* m_GameLayer = nullptr;
};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args);