#pragma once

#include "BomberMan/Core/AudioManager.h"
#include "BomberMan/Game/GCamera.h"
#include "BomberMan/Game/Grid.h"
#include "BomberMan/Game/Player.h"
#include "BomberMan/Game/Bomb.h"
#include "BomberMan/Game/Enemy.h"
#include "BomberMan/UI/FXManager.h"
#include "BomberMan/UI/SimpleUI.h"

#include <Hurikan/Scene/Scene.h>

using namespace Hurikan;

class InGame : public Observer, public Observable
{
public:
	~InGame();

	// Initializes at the start of the application
	void Init();
	// Loads a level
	void Load();
	// Render and game logic
	void OnUpdate(Timestep& ts);

	void OnGameEvent(GameEvent& e) override;

	void Pause(bool pause);
	bool Paused() const { return m_Paused; }

private:
	Scene m_InGameScene;
	
	GCamera m_GameCamera;
	Grid m_Grid;
	Player m_Player;
	BombManager m_BombManager;
	EnemySpawner m_EnemySpawner;
	FXManager m_FXManager;
	AudioAssistant m_AudioAssistant;

	//Navigation m_Navigation;
	// Test
	SimpleUI m_SimpleUI;

	bool m_Paused = false;

	// Maybe abstract vvvvvv
	unsigned short m_PressedKey, m_LastPressedKey;
};