#include "GCamera.hpp"

#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/Utils.hpp"
#include "BomberMan/Game/Grid.hpp"

#include <Hurikan/Core/Application.h>
#include <Hurikan/Scene/Components.h>

#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>

void GCamera::Create(Scene* scene)
{
	g_GameScene = scene;

	m_PlayerCamera = g_GameScene->CreateEntityWithDrawOrder(0);
	m_CutSceneCamera = g_GameScene->CreateEntityWithDrawOrder(0);
	m_BlackSquare = g_GameScene->CreateEntityWithDrawOrder(4);
	
	m_BlackSquare.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
	m_BlackSquare.Transform().Scale *= 15;

	constexpr float orthoSize = 18;
	const auto& [width, height] = Application::Get().GetWindowSize();
	{
		auto& player_camera_cmp = m_PlayerCamera.AddComponent<CameraComponent>();
		player_camera_cmp.Camera.SetViewportSize(width, height);
		player_camera_cmp.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		player_camera_cmp.Camera.SetOrthographicSize(orthoSize);
		player_camera_cmp.Primary = true;
	}

	{
		auto& zoom_camera_cmp = m_CutSceneCamera.AddComponent<CameraComponent>();
		zoom_camera_cmp.Camera.SetViewportSize(width, height);
		zoom_camera_cmp.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		zoom_camera_cmp.Camera.SetOrthographicSize(orthoSize);
		zoom_camera_cmp.Primary = false;
	}
}

void GCamera::OnGameEvent(GameEvent& e)
{
	if (m_GameOver)
		return;

	if (e.Type == GameEventType::PLAYER_SUCCESS_EXIT)
	{
		m_GameOver = true; 
		return;
	}

	if (m_GameOver && !SaveLoadSystem::GetCurrentLevel().OverScreen())
		return;

	if (e.Type == GameEventType::PLAYER_MOVED || e.Type == GameEventType::PLAYER_START_POSITION)
	{
		m_PlayerPosition = std::any_cast<glm::vec3>(e.Data);

		m_PlayerCamera.Transform().Translation.x = glm::clamp(m_PlayerPosition.x, 0.0f,
			Utils::RoundUp((SaveLoadSystem::GetCurrentLevel().Width - (float)MIN_LEVEL_WIDTH)));

		m_PlayerCamera.Transform().Translation.y = glm::clamp(m_PlayerPosition.y, Utils::RoundUp((SaveLoadSystem::GetCurrentLevel().Height - MIN_LEVEL_HEIGHT) / -2.0f), 
											Utils::RoundUp((SaveLoadSystem::GetCurrentLevel().Height - MIN_LEVEL_HEIGHT) / 2.0f));
	}
	else if (e.Type == GameEventType::ENEMY_CREATE_NEW)
	{
		auto& data = std::any_cast<Entity>(e.Data);

		if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_BOSS)
		{
			m_BossFightZoom = true;
			m_BossFight = true;
			m_PlayerCamera.GetComponent<CameraComponent>().Primary = false; // Start the "cutscene"
			m_CutSceneCamera.GetComponent<CameraComponent>().Primary = true;
			m_BossEntity = data;
		}
	}
}

void GCamera::OnUpdate(Timestep& ts)
{
	if (!m_BossFight)
		return;

	if (m_BossFightZoom) {

		auto& transform = m_CutSceneCamera.Transform();
		transform.Translation.x = Utils::Lerp(transform.Translation.x, m_BossEntity.Transform().Translation.x, ts * 4);
		//transform.Translation.y = Utils::Lerp(transform.Translation.y, m_BossEntity.Transform().Translation.y, ts * 4);

		m_BlackSquare.Transform().Translation = transform.Translation;

		auto& camera = m_CutSceneCamera.GetComponent<CameraComponent>().Camera;
		camera.SetOrthographicSize(Utils::Lerp(camera.GetOrthographicSize(), 6, ts * 5));

		if (transform.Translation.x == m_BossEntity.Transform().Translation.x && camera.GetOrthographicSize() == 6)
		{
			auto& color = m_BlackSquare.GetComponent<SpriteRendererComponent>().Color;

			color.a =  Utils::Lerp(color.a, 1.0f, ts);

			m_BlackSquare.GetComponent<SpriteRendererComponent>().Color = color;

			if (color.a == 1.0f)
			{
				m_BossFightZoom = false;

				m_PlayerCamera.GetComponent<CameraComponent>().Primary = true;
				m_CutSceneCamera.GetComponent<CameraComponent>().Primary = false;
				m_BlackSquare.Transform().Translation.x = 0;
				m_BlackSquare.Transform().Translation.y = 0;
				m_BlackSquare.Transform().Scale.x *= 4;
				m_BlackSquare.Transform().Scale.y = 20;
			}
			// Camera zoomed in on boss and now zoom out to default position
		}
	}
	else
	{
		auto& color = m_BlackSquare.GetComponent<SpriteRendererComponent>().Color;
		if (color.a > 0.0f)
		{
			color.a = Utils::Lerp(color.a, 0.0f, ts);
		}
		else if (color.a == 0.0f)
		{ 
			m_BossFight = false;
			Dispatch(GameEventType::CUTSCENE_COMPLETED);
		}
	}
}
