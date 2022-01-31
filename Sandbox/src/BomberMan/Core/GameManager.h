#pragma once

#include "BomberMan/Game/InGame.h"

#include <Hurikan/Core/Layer.h>
#include <Hurikan/Events/KeyEvent.h>
using namespace Hurikan;

enum class SceneType
{
	MAIN_MENU = 0,
	LOADING_LEVEL,
	IN_GAME,
	FAIL_LEVEL,
	SUCCESS_LEVEL
};

class MainMenu;

class GameManager : public Layer, public Observer
{
public:
	GameManager() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Timestep& ts) override;
	void OnImGuiRender() override;

	void OnEvent(Event& e) override;

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);

	void OnGameEvent(GameEvent& e) override;
private:
	InGame m_Game;
	MainMenu* m_MainMenu;

	SceneType m_CurrentScreen = SceneType::MAIN_MENU;
};