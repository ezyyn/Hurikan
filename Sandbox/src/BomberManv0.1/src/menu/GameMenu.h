#pragma once

#include "../sceneobjects/GameCamera.h"
#include "../core/GameManager.h"

#include <Hurikan.h>
using namespace Hurikan;

enum class GameState
{
	MENU = 0,
	LEVEL_START,
	INGAME,
	LEVEL_SUCCESS,
	GAMEOVER,
};

enum class GameOptions
{
	NONE = -1,
	NEW_GAME = 0,
	CONTINUE,
	SETTIGS,
	STATISTICS,
	EXIT
};

class GameMenu
{
public:
	GameMenu() = default;
	~GameMenu() = default;

	void Init(uint32_t width, uint32_t height);
	void OnUpdate(Timestep ts);

	void Confirm();
	bool Closed() { return m_ConfirmedOption == GameOptions::EXIT; }
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	Scene m_MenuScene;
	GameCamera m_MenuCamera;

	Entity m_ArrowHead;

	GameManager m_GameManager;

	uint32_t m_SelectedOptionIndex = 0;
	GameOptions m_SelectedOption = GameOptions::NEW_GAME;
	GameOptions m_ConfirmedOption = GameOptions::NONE;

	GameState m_GameState = GameState::MENU;
};
