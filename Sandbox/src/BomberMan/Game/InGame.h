#pragma once

#include "BomberMan/Game/GCamera.h"
#include "BomberMan/Game/Grid.h"
#include "BomberMan/Game/Player.h"
#include "BomberMan/Game/Bomb.h"
#include "BomberMan/Game/Enemy.h"

#include "BomberMan/UI/SimpleUI.h"
//#include "BomberMan/Core/Navigation.h"

#include <Hurikan/Scene/Scene.h>
using namespace Hurikan;

class InGame
{
public:
	// Initializes at the start of the application
	void Init();
	// Loads a level
	void Load();
	// Render and game logic
	void OnUpdate(Timestep& ts);
private:
	Scene m_InGameScene;
	
	GCamera m_GameCamera;
	Grid m_Grid;
	Player m_Player;
	BombManager m_BombManager;
	EnemySpawner m_EnemySpawner;

	//Navigation m_Navigation;
	// Test
	SimpleUI m_SimpleUI;

	// Maybe abstract vvvvvv
	unsigned short m_PressedKey, m_LastPressedKey;
};