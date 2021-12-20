#pragma once

#include "../ui/MainMenu.h"
#include "../sceneobjects/Game.h"

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

	GameState m_GameState = GameState::MENU;
};

