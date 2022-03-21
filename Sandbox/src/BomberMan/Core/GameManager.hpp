/*
 * Whole game is a combination of the entity component system and Observer design pattern.
 * 
 * Each "screen" has it's own scene. That scene renders all the entities with a sprite renderer component
 * 
 * GameManager class
 * 
 */

#pragma once

#include "BomberMan/UI/MainMenu.hpp"
#include "BomberMan/Core/AudioManager.hpp"
#include "BomberMan/Core/Utils.hpp"

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
	LEVEL_SUCCESS,
	GAME_END_TEXT
};

class GameManager : public Layer, public Observer, public Observable
{
public:
	GameManager() = default;
	~GameManager() = default;

	// Functions inherited from Layer
	void OnAttach() override;
	void OnDetach() override;
	// Used for game logic and also render
	void OnUpdate(Timestep& ts) override;
	// Used mainly for debugging in this game
	void OnImGuiRender() override;
	// This func gets called every time an event happened, f.e. mouse click
	void OnEvent(Event& e) override;
	// Called by EventDispatcher in OnEvent func, return true if the event was handled
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	// Function inherited from Observer (see Observer.h for more details)
	void OnGameEvent(GameEvent& e) override;
private:
	InGame* m_Game = nullptr;
	MainMenu m_MainMenu;
	AudioAssistant m_AudioAssistant;
	SceneType m_CurrentScreen;

	// Timers for scene transitions
	Utils::Timer<SceneType> m_NextLevelTimer;
	Utils::Timer<SceneType> m_LoadLevelTimer;
};