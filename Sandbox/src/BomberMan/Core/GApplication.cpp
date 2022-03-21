/*
 *
 * This is where the program starts.
 * 
 * File "Hurikan/Core/EntryPoint.h" contains the main method and CreateApplication definition.
 *
 * Whole game engine is build up on a layer system. Each Layer has its own functionality.
 * 
 * In this stage of my game engine there is not a lot of layers. For Debug configuration there is also a ImGui layer. Which I used for debugging and editing purposes.
 * In Release configuration there is no such thing and the only layer that is present is my game.
 * 
 * 
*/

#include "BomberMan/Core/GameManager.hpp"
#include <Hurikan/Core/EntryPoint.h>

class GApplication : public Hurikan::Application
{
public:
	GApplication(const Hurikan::ApplicationSpecification& specification, 
		Hurikan::ApplicationCommandLineArgs args) 
		: Application(specification, args)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new GameManager);
	}
};

Hurikan::Application* Hurikan::CreateApplication(Hurikan::ApplicationCommandLineArgs args)
{
	Hurikan::ApplicationSpecification specification;
	specification.Name = "BomberMan v0.3";
	specification.WindowWidth = 1600;
	specification.WindowHeight = 900;
	specification.StartMaximized = false;
	specification.VSync = true;
#ifdef HU_DEBUG
	specification.EnableImGui = true;
#else
	specification.EnableImGui = false;
#endif
	specification.Resizable = false;

	return new GApplication(specification, args);
}