#include "InGame.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/AudioManager.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>
#include <Hurikan/Core/Application.h>

extern GameData g_InGameData = GameData();

InGame::~InGame()
{
}

void InGame::Init(AudioAssistant& assistant)
{
	g_InGameData = SaveLoadSystem::GetGameData();

	// Background Image
	{
		auto& bckg = m_InGameScene.CreateEntityWithDrawOrder(0);

		auto& src = bckg.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f,0.8f, 1.0f, 1.0f));
		src.Texture = ResourceManager::GetTexture("Background");
		if (SaveLoadSystem::GetCurrentLevel().Width > SaveLoadSystem::GetCurrentLevel().Height)
		{
			bckg.GetComponent<TransformComponent>().Scale = { SaveLoadSystem::GetCurrentLevel().Width * 2, SaveLoadSystem::GetCurrentLevel().Width  * 2, 0.0f };
			bckg.GetComponent<SpriteRendererComponent>().TilingFactor = (float)SaveLoadSystem::GetCurrentLevel().Width  * 2;
		} 
		else
		{
			bckg.GetComponent<TransformComponent>().Scale = { SaveLoadSystem::GetCurrentLevel().Height  * 2, SaveLoadSystem::GetCurrentLevel().Height * 2, 0.0f };
			bckg.GetComponent<SpriteRendererComponent>().TilingFactor = (float)SaveLoadSystem::GetCurrentLevel().Height * 2;
		}
	}

	m_FXManager.Init(&m_InGameScene);
	m_SimpleUI.Init();
	m_BombManager.Init(&m_InGameScene);
	m_EnemySpawner.Init(&m_InGameScene);

	m_GameCamera.Create(&m_InGameScene);

	{
		// InGame's listeners
		Attach(&assistant);

		// Camera's listeners
		m_GameCamera.Attach(&assistant);

		// Grid's listeners
		m_Grid.Attach(&m_Player);
		m_Grid.Attach(&m_BombManager);
		m_Grid.Attach(&m_EnemySpawner);
		m_Grid.Attach(&m_FXManager);
		m_Grid.Attach(&m_GameCamera);
		m_Grid.Attach(&m_SimpleUI);
		m_Grid.Attach(&assistant);
		m_Grid.Attach(this);

		// Player's listeners
		m_Player.Attach(&m_Grid);
		m_Player.Attach(&m_GameCamera);
		m_Player.Attach(&m_BombManager);
		m_Player.Attach(&m_SimpleUI);
		m_Player.Attach(&assistant);
		m_Player.Attach(&m_EnemySpawner);
		m_Player.Attach(this);

		// BombManagers's listeners
		m_BombManager.Attach(&m_Grid);
		m_BombManager.Attach(&m_Player);
		m_BombManager.Attach(&m_EnemySpawner);
		m_BombManager.Attach(&m_FXManager);
		m_BombManager.Attach(&m_SimpleUI);
		m_BombManager.Attach(&assistant);
		// EnemySpawner's listeners
		m_EnemySpawner.Attach(&m_Player);
		m_EnemySpawner.Attach(&m_FXManager);
		m_EnemySpawner.Attach(&assistant);
		m_EnemySpawner.Attach(&m_Grid);
		m_EnemySpawner.Attach(this);

		// Inititializing
		m_Player.Create(m_InGameScene);
		m_Grid.Create(&m_InGameScene);

		// SimpleUI's listeners
		m_SimpleUI.Attach(this);
		m_SimpleUI.Attach(&assistant);
		m_InGameScene.OnRuntimeStart();
	}
	if(!SaveLoadSystem::GetCurrentLevel().BossLevel)
		Dispatch(GameEventType::AUDIO_INGAME_LOOP);
}

void InGame::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::PLAYER_GONE)
	{
		Dispatch(GameEventType::LEVEL_FAILED);
	}
	else if (e.Type == GameEventType::PLAYER_SUCCESS_EXIT)
	{
		g_InGameData.CompletedLevels++;
		SaveLoadSystem::SaveLevel(g_InGameData);
		Dispatch(GameEventType::LEVEL_SUCCESS);
	}
	else if (e.Type == GameEventType::RETURN_TO_MAIN_MENU_CONFIRMED)
	{
		Dispatch(GameEventType::RETURN_TO_MAIN_MENU);
	}
	else if (e.Type == GameEventType::ENEMY_DEAD)
	{
		auto& data = std::any_cast<Entity>(e.Data);

		if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_REGULAR)
		{
			g_InGameData.Score += 100;
		} 
		else if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_FAST)
		{
			g_InGameData.Score += 300;
		}
		else if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_SMART)
		{
			g_InGameData.Score += 500;
		}
	}
}

void InGame::Pause(bool pause)
{
	if (pause == m_Paused)
		return;

	m_Paused = pause;
	if (m_Paused)
	{
		m_SimpleUI.DisplayPauseMenu();
		if (!SaveLoadSystem::GetCurrentLevel().BossLevel)
			Dispatch(GameEventType::AUDIO_PAUSE_INGAME_LOOP);
	}
	else
	{
		m_SimpleUI.HidePauseMenu();
		if (!SaveLoadSystem::GetCurrentLevel().BossLevel)
			Dispatch(GameEventType::AUDIO_UNPAUSE_INGAME_LOOP);
	}
}

void InGame::OnUpdate(Timestep& ts)
{
	// Rendering
	m_InGameScene.OnUpdateRuntime(ts);
	m_SimpleUI.OnUpdate(ts,m_Paused);
	m_GameCamera.OnUpdate(ts);
	// Logic
	if (!m_Paused)
	{
		m_FXManager.OnUpdate(ts);
		m_EnemySpawner.OnUpdate(ts);
		m_Grid.OnUpdate(ts);
		m_BombManager.OnUpdate(ts);
		m_Player.OnUpdate(ts);
	}
}
