#pragma once
#include "../core/CollisionDetector.h"

#include "../sceneobjects/GameCamera.h"
#include "../sceneobjects/GameGrid.h"
#include "../sceneobjects/Player.h"
#include "../sceneobjects/Enemy.h"

#include <Hurikan.h>
using namespace Hurikan;

// TODO: for future make this center of everything

class GameManager
{
public:
	GameManager() = default;
	~GameManager() = default;

	void Init(uint32_t rows, uint32_t columns);
	void OnUpdate(Timestep ts);
	void Nuke();

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
public:
	bool GameOver = false;
private:
	Scene* m_InGameScene = nullptr;

	GameGrid m_GameGrid;
	Player m_Player;
	EnemySpawner m_Enemy;
	CollissionDetector m_CollisionDetector;
	GameCamera m_GameCamera;

};