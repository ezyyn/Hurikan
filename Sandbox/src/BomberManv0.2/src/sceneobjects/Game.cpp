#include "Game.h"

#include "../sceneobjects/Player.h"

extern Player* g_Player = new Player;
extern Grid* g_GameGrid = new Grid;
extern Scene* g_GameScene = new Scene;

void Game::Init()
{
	// Camera settings
	m_Camera = g_GameScene->CreateEntity();

	const auto& [width, height] = Application::Get().GetWindowSize();
	m_Camera.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);

	auto& camera_cmp = m_Camera.GetComponent<CameraComponent>();
	camera_cmp.FixedAspectRatio = true;
	auto& camera = camera_cmp.Camera;
	camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	camera.SetOrthographicSize(18);

	m_CameraTransform = &m_Camera.Transform();
	m_CameraTransform->Translation.x = 15.5f;
	m_CameraTransform->Translation.y = -7.5f;

	// GameGrid settings
	g_GameGrid->Init();
	g_Player->Init();
}

void Game::Play(const Level& level)
{
	m_CurrentLevel = level;

	glm::vec2 startupPosition = {};
	g_GameGrid->Generate(level, &startupPosition);
	g_Player->SetStartPosition(startupPosition);

	m_BombManager.Init(m_CurrentLevel);

	g_GameScene->OnRuntimeStart();
}

void Game::OnUpdate(Timestep ts)
{
	// Camera follows player
	/*
	if (m_PlayerTransform->Translation.x >= 15.5f && m_PlayerTransform->Translation.x <= m_CurrentLevel.Width - 15.5f)
		m_CameraTransform.Translation.x = m_PlayerTransform->Translation.x;
	*/

	g_Player->OnUpdate(ts);
	m_BombManager.OnUpdate(ts);
	g_GameGrid->OnUpdate(ts);

	g_GameScene->OnUpdateRuntime(ts);
}

void Game::Shutdown()
{
	delete g_Player;
	g_Player = nullptr;

	delete g_GameGrid;
	g_GameGrid = nullptr;

	g_GameScene->OnRuntimeStop();
	delete g_GameScene;
}

KeyCode lastKey1 = 0;

void Game::OnKeyPressed(KeyPressedEvent& e)
{
	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			if (lastKey1 != Key::B)
			{
				BombProperties props;
				props.Type = BombType::CLASSIC;
				props.ExplosionTime = 3.0f;
				props.Reach = 3;
				props.State = BombState::TICKING;

				props.StartPosition = g_Player->RoundPosition();
				props.CurrentLevel = m_CurrentLevel;
				m_BombManager.PlaceBomb(props);
			}
			break;
		}
	}

}
void Game::OnKeyReleased(KeyReleasedEvent& e)
{
	lastKey1 = 0;
}