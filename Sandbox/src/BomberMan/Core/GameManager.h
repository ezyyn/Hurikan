#pragma once

#include "BomberMan/UI/MainMenu.h"
#include "BomberMan/Core/AudioManager.h"

#include <Hurikan/Core/Layer.h>
#include <Hurikan/Events/KeyEvent.h>
using namespace Hurikan;

class InGame;

enum class SceneType
{
	MAIN_MENU = 0,
	RETURN_TO_MAIN_MENU,
	LOADING_LEVEL,
	IN_GAME,
	LEVEL_FAIL,
	LEVEL_SUCCESS
};

class GameManager : public Layer, public Observer
{
public:
	GameManager() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Timestep& ts) override;
	void WaitAndSwitch(SceneType type, Timestep& ts);
	void LoadLevel(Timestep& ts);

	void OnImGuiRender() override;

	void OnEvent(Event& e) override;

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);

	void OnGameEvent(GameEvent& e) override;
private:
	InGame* m_Game = nullptr;
	MainMenu m_MainMenu;
	AudioAssistant m_AudioAssistant;

	unsigned short m_KeyPressed = 0;

	Entity m_LevelCount;
	Scene m_LoadLevelScene;

	SceneType m_CurrentScreen;
};