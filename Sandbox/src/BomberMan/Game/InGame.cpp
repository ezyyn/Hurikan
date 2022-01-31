#include "InGame.h"

#include "BomberMan/Core/ResourceManager.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>
#include <Hurikan/Core/Application.h>

void InGame::Init()
{
	ResourceManager::Init();

	// Background Image
	{
		auto& bckg = m_InGameScene.CreateEntityWithDrawOrder(0);

		auto& src = bckg.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
		src.Texture = ResourceManager::GetTexture("Background");
		bckg.GetComponent<TransformComponent>().Scale = { 32, 32, 0.0f };
		//bckg.GetComponent<TransformComponent>().Translation = { glm::round(m_CurrentLevel.Width / 2), -glm::round(m_CurrentLevel.Height / 2) - 1, 0 };
		bckg.GetComponent<SpriteRendererComponent>().TilingFactor = 32.0f;
	}

	m_GameCamera.Create(m_InGameScene.CreateEntity());

	m_SimpleUI.Init();
	m_BombManager.Init(&m_InGameScene);
	m_EnemySpawner.Init(&m_InGameScene);
}

void InGame::Load()
{
	m_Grid.Attach(&m_Player);
	m_Grid.Attach(&m_BombManager);
	m_Grid.Attach(&m_EnemySpawner);

	// Player => GRID LISTENS TO PLAYER
	m_Player.Attach(&m_Grid);
	m_Player.Attach(&m_GameCamera);
	m_Player.Attach(&m_SimpleUI);

	// Bomb manager
	m_BombManager.Attach(&m_Grid);
	m_BombManager.Attach(&m_Player);
	m_BombManager.Attach(&m_EnemySpawner);
	m_BombManager.Attach(&m_SimpleUI);

	m_Player.Create(m_InGameScene);
	m_Grid.Create(m_InGameScene);

	m_InGameScene.OnRuntimeStart();
}

void InGame::OnUpdate(Timestep& ts)
{
	{ // Maybe abstract vvvv
		static bool Released = true;

		if (Released && Input::IsKeyPressed(Key::B))
		{
			Released = false;

			BombProps props;

			props.ExplosionTime = 3.0f;
			props.Reach = 1;
			props.Type = BombType::CLASSIC;
			m_BombManager.PlaceBomb(props);
		}

		Released = Input::IsKeyReleased(Key::B);
	}

	m_EnemySpawner.OnUpdate(ts);
	m_Grid.OnUpdate(ts);
	m_BombManager.OnUpdate(ts);
	m_Player.OnUpdate(ts);
	m_InGameScene.OnUpdateRuntime(ts);

	m_SimpleUI.OnUpdate(ts);
}
