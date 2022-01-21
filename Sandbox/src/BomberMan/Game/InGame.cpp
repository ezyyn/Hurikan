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
		bckg.GetComponent<TransformComponent>().Scale = { 16, 16, 0.0f };
		//bckg.GetComponent<TransformComponent>().Translation = { glm::round(m_CurrentLevel.Width / 2), -glm::round(m_CurrentLevel.Height / 2) - 1, 0 };
		bckg.GetComponent<SpriteRendererComponent>().TilingFactor = 16.0f;
	}
	// Camera init
	m_GameCamera.Create(m_InGameScene.CreateEntity());

	m_BombManager.Init(&m_InGameScene);
}

void InGame::Load()
{
	// Grid's subscribtions
	m_Grid.Attach(&m_Player);
	m_Grid.Attach(&m_BombManager);

	// Player's subscribtions
	m_Player.Attach(&m_Grid);

	// Bomb Manager's subscribtions
	m_BombManager.Attach(&m_Grid);

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

	m_BombManager.OnUpdate(ts);
	m_Player.OnUpdate(ts);
	m_InGameScene.OnUpdateRuntime(ts);
}
