#include "Game.h"

#include "../sceneobjects/Player.h"
#include "../sceneobjects/Grid.h"

extern Player* g_Player = new Player;
extern Grid* g_GameGrid = new Grid;
extern Scene* g_GameScene = new Scene;

constexpr float orthoSize = 18;

void Game::Init()
{
	ResourceManager::Init("assets/textures/");

	// Camera settings
	m_Camera = g_GameScene->CreateEntity();

	const auto& [width, height] = Application::Get().GetWindowSize();
	m_Camera.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);

	auto& camera_cmp = m_Camera.GetComponent<CameraComponent>();
	camera_cmp.FixedAspectRatio = true;
	auto& camera = camera_cmp.Camera;
	camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	camera.SetOrthographicSize(orthoSize);

	m_CameraTransform = &m_Camera.Transform();

	g_GameGrid->Init();
	g_Player->Init();
}

extern float camera_position_x = 0.0f;
extern float camera_position_y = 0.0f;

void Game::Play(const Level& level)
{
	m_CurrentLevel = level;

	glm::vec2 startupPosition = {};
	g_GameGrid->Generate(level, &startupPosition);
	g_Player->SetStartPosition(startupPosition);
	m_BombManager.Init(m_CurrentLevel);

	float vertExtent = orthoSize;
	float horzExtent = vertExtent * 1600.0f / 900.0f;

	m_Boundaries.MinX = 15.5f;
	m_Boundaries.MinY = -0.5f;
	m_Boundaries.MaxX = g_GameGrid->GetLevelWidth() - m_Boundaries.MinX - 1;
	m_Boundaries.MaxY = 5.1f;

	camera_position_x = glm::clamp(g_Player->Position2().x, m_Boundaries.MinX, m_Boundaries.MaxX);
	camera_position_y = glm::clamp(g_Player->Position2().y, m_Boundaries.MinY, m_Boundaries.MaxY);

	m_CameraTransform->Translation.x = camera_position_x;
	m_CameraTransform->Translation.y = camera_position_y;

	HU_INFO(camera_position_x);
	HU_INFO(m_CameraTransform->Translation.y);

	m_UI.Init(m_CurrentLevel);

	g_GameScene->OnRuntimeStart();
}

inline float mylerp(float start, float end, float maxDistanceDelta)
{
	if (glm::abs(end - start) <= maxDistanceDelta)
		return end;

	return start + glm::sign(end - start) * maxDistanceDelta;
}

void Game::OnUpdate(Timestep ts)
{
	// Placing a bomb
	if (Input::IsKeyPressed(Key::B))
	{
		if (m_LastKeyPressed != Key::B)
		{
			m_LastKeyPressed = Key::B;

			BombProperties props;
			props.Type = BombType::CLASSIC;
			props.ExplosionTime = 3.0f;
			props.Reach = 1;
			props.State = BombState::TICKING;

			props.StartPosition = g_Player->RoundPosition();
			props.CurrentLevel = m_CurrentLevel;
			m_BombManager.PlaceBomb(props);
		}
	}
	else if (Input::IsKeyReleased(Key::B))
	{
		m_LastKeyPressed = 0;
	}

	m_BombManager.OnUpdate(ts);

	g_Player->OnUpdate(ts);

	g_GameGrid->OnUpdate(ts);

	// Camera follows player
	if (g_Player->IsMoving())
	{
		camera_position_x = glm::clamp(g_Player->Position2().x, m_Boundaries.MinX, m_Boundaries.MaxX);
		camera_position_y = glm::clamp(g_Player->Position2().y, m_Boundaries.MinY, m_Boundaries.MaxY);

		m_CameraTransform->Translation.x = camera_position_x;
		m_CameraTransform->Translation.y = camera_position_y;
	}

	g_GameScene->OnUpdateRuntime(ts);

	m_UI.OnUpdate(ts);
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