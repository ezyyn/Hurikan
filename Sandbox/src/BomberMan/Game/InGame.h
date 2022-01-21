#pragma once

#include "BomberMan/Game/GCamera.h"
#include "BomberMan/Game/Grid.h"
#include "BomberMan/Game/Player.h"
#include "BomberMan/Game/Bomb.h"

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

	// Maybe abstract vvvvvv
	unsigned short m_PressedKey, m_LastPressedKey;
};