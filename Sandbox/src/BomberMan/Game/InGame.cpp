#include "InGame.h"

#include "BomberMan/Core/ResourceManager.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>
#include <Hurikan/Core/Application.h>

InGame::~InGame()
{
}

void InGame::Init()
{
	// Background Image
	{
		auto& bckg = m_InGameScene.CreateEntityWithDrawOrder(0);

		auto& src = bckg.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f,0.0f, 1.0f, 1.0f));
		src.Texture = ResourceManager::GetTexture("Background");
		if (SaveManager::GetCurrentLevel().Width > SaveManager::GetCurrentLevel().Height)
		{
			bckg.GetComponent<TransformComponent>().Scale = { SaveManager::GetCurrentLevel().Width, SaveManager::GetCurrentLevel().Width, 0.0f };
			bckg.GetComponent<SpriteRendererComponent>().TilingFactor = SaveManager::GetCurrentLevel().Width;
		} 
		else
		{
			bckg.GetComponent<TransformComponent>().Scale = { SaveManager::GetCurrentLevel().Height, SaveManager::GetCurrentLevel().Height, 0.0f };
			bckg.GetComponent<SpriteRendererComponent>().TilingFactor = SaveManager::GetCurrentLevel().Height;
		}
	
	}

	m_FXManager.Init(&m_InGameScene);
	m_SimpleUI.Init();
	m_BombManager.Init(&m_InGameScene);
	m_EnemySpawner.Init(&m_InGameScene);

	m_GameCamera.Create(m_InGameScene.CreateEntityWithDrawOrder(0));
}

void InGame::Load()
{
	// Grid's listeners
	m_Grid.Attach(&m_Player);
	m_Grid.Attach(&m_BombManager);
	m_Grid.Attach(&m_EnemySpawner);
	m_Grid.Attach(&m_FXManager);
	m_Grid.Attach(&m_GameCamera);
	m_Grid.Attach(&m_SimpleUI);
	m_Grid.Attach(this);

	// Player's listeners
	m_Player.Attach(&m_Grid);
	m_Player.Attach(&m_GameCamera);
	m_Player.Attach(&m_BombManager);
	m_Player.Attach(&m_SimpleUI);
	m_Player.Attach(this);

	// BombManagers's listeners
	m_BombManager.Attach(&m_Grid);
	m_BombManager.Attach(&m_Player);
	m_BombManager.Attach(&m_EnemySpawner);
	m_BombManager.Attach(&m_FXManager);
	// EnemySpawner's listeners
	m_EnemySpawner.Attach(&m_Player);
	m_EnemySpawner.Attach(&m_FXManager);

	// Inititializing
	m_Player.Create(m_InGameScene);
	m_Grid.Create(m_InGameScene);

	// SimpleUI's listeners
	m_SimpleUI.Attach(this);
	m_InGameScene.OnRuntimeStart();
}

void InGame::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::GAME_LOST)
	{
		Dispatch(GameEventType::GAME_LOST);
	}
	else if (e.Type == GameEventType::GAME_WON)
	{
		Dispatch(GameEventType::GAME_WON);
	}
	else if (e.Type == GameEventType::RETURN_TO_MAIN_MENU)
	{
		Dispatch(GameEventType::RETURN_TO_MAIN_MENU);
	}
}

void InGame::Pause(bool pause)
{
	if (pause == m_Paused)
		return;

	m_Paused = pause;
	if (m_Paused)
		m_SimpleUI.DisplayPauseMenu();
	else
		m_SimpleUI.HidePauseMenu();
}

void InGame::OnUpdate(Timestep& ts)
{
	m_InGameScene.OnUpdateRuntime(ts);
	m_SimpleUI.OnUpdate(ts,m_Paused);
	if (!m_Paused)
	{
		m_FXManager.OnUpdate(ts);
		m_EnemySpawner.OnUpdate(ts);
		m_Grid.OnUpdate(ts);
		m_BombManager.OnUpdate(ts);
		m_Player.OnUpdate(ts);
	}
}
