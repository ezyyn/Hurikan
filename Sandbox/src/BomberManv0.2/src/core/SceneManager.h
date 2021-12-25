#pragma once

#include "../ui/MainMenu.h"
#include "../sceneobjects/Game.h"

//#define EXPRM 1

#ifdef EXPRM
	#include "../test/TestScene.h"
#endif
enum class GameState
{
	MENU = 0, INGAME
};

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	void Init();
	void Shutdown();
	void OnUpdate(Timestep ts);

	void OnEvent(Event& e);
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);

	void Switch(GameState state);
private:
	MainMenu m_MainMenu;
	Game m_Game;
#ifdef EXPRM
	TestScene m_TestScene;
#endif
	GameState m_GameState = GameState::MENU;
};

