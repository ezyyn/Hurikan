#pragma once

#include <Hurikan.h>
using namespace Hurikan;

enum class GameOptions
{
	NONE = -1,
	NEW_GAME = 0,
	CONTINUE,
	SETTINGS,
	STATISTICS,
	EXIT
};

class SceneManager;

class MainMenu
{
public:
	MainMenu() = default;
	~MainMenu() = default;

	void Init(SceneManager* sm);
	void Shutdown();
	void OnUpdate(Timestep ts);

	void Confirm();

	void OnKeyPressed(KeyPressedEvent& e);
	void OnKeyReleased(KeyReleasedEvent& e);
private:
	Scene m_MenuScene;
	Entity m_Camera;
	Entity m_ArrowHead;

	GameOptions m_ConfirmedOption = GameOptions::NONE;
	GameOptions m_SelectedOption = GameOptions::NEW_GAME;
	uint32_t m_SelectedOptionIndex = 0;
private:
	SceneManager* g_SceneManager;
};

/*
enum class GameState
{
	MENU = 0,
	LEVEL_START,
	INGAME,
	LEVEL_SUCCESS,
	GAMEOVER,
};

class Menu
{
public:
	Menu() = default;
	~Menu() = default;

	void Init();
	void OnUpdate(Timestep ts);

	void Confirm();
	bool Closed() { return m_ConfirmedOption == GameOptions::EXIT; }
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	Scene m_MenuScene;
	GameCamera m_MenuCamera;

	Entity m_ArrowHead;

	Game m_GameManager;

	uint32_t m_SelectedOptionIndex = 0;
	GameOptions m_SelectedOption = GameOptions::NEW_GAME,
				m_ConfirmedOption = GameOptions::NONE;

	std::vector<Level> m_Levels;

	GameState m_GameState = GameState::MENU;
};*/