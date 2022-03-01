#pragma once

#include "BomberMan/Game/GCamera.hpp"
#include "BomberMan/Game/Grid.hpp"
#include "BomberMan/Game/Player.hpp"
#include "BomberMan/Game/Bomb.hpp"
#include "BomberMan/Game/Enemy.hpp"
#include "BomberMan/UI/FXManager.hpp"
#include "BomberMan/UI/SimpleUI.hpp"

#include <Hurikan/Scene/Scene.h>

using namespace Hurikan;

class AudioAssistant;

class InGame : public Observer, public Observable
{
public:
	~InGame();

	void Init(AudioAssistant& assistant);

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

	SimpleUI m_SimpleUI;

	bool m_Paused = false;

	// Maybe abstract vvvvvv
	unsigned short m_PressedKey, m_LastPressedKey;
};