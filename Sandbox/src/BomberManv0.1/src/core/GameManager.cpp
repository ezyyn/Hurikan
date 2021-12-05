#include "GameManager.h"

#include "../sceneobjects/Bomb.h"

void GameManager::Init(uint32_t rows, uint32_t columns)
{
	Nuke();

	GameOver = false;
	m_InGameScene = new Scene();

	// Initilaization of scene objects
	m_GameCamera.Init(m_InGameScene, 1600, 900);
	m_GameGrid.Init(m_InGameScene, &m_Player, rows, columns);
	m_Player.Init(m_InGameScene, &m_GameGrid);
	m_Enemy.Init(m_InGameScene, &m_GameGrid, &m_Player);
	// Initializing scene
	m_InGameScene->OnRuntimeStart();
	
	m_CollisionDetector.Init(m_InGameScene);
	m_InGameScene->SetContactListener((b2ContactListener*)&m_CollisionDetector);
}

float timer = 0.0f;
float desiredTime = 5000.0f;

void GameManager::OnUpdate(Timestep ts)
{
	if (!GameOver && m_Player.stats().Health <= 0)
	{
		// Check if animation is complete
		timer += ts;
		if (timer >= desiredTime / 1000)
		{
			timer = 0.0f;
			GameOver = true;
			this->Nuke();
		}
	}

	if (GameOver)
		return;

	// Updating game logic
	m_Player.OnUpdate(ts);
	m_GameGrid.OnUpdate(ts);

	// Rendering and updating physics
	m_InGameScene->OnUpdateRuntime(ts);
}

void GameManager::Nuke()
{
	// TODO: Maybe just reset values 
	if (m_InGameScene != nullptr)
	{
		delete m_InGameScene;
		m_InGameScene = nullptr;
	}
}

bool GameManager::OnKeyPressed(KeyPressedEvent& e)
{
	m_Player.OnKeyPressed(e);

	return false;
}

bool GameManager::OnKeyReleased(KeyReleasedEvent& e)
{
	m_Player.OnKeyReleased(e);

	return false;
}
