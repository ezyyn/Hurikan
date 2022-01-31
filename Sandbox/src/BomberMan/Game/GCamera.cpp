#include "GCamera.h"

#include <BomberMan/Core/LevelManager.h>

#include <Hurikan/Core/Application.h>
#include <Hurikan/Scene/Components.h>

void GCamera::Create(Entity& e)
{
	m_Handle = e;

	constexpr float orthoSize = 18;

	const auto& [width, height] = Application::Get().GetWindowSize();

	auto& camera_cmp = m_Handle.AddComponent<CameraComponent>();
	camera_cmp.Camera.SetViewportSize(width, height);
	camera_cmp.FixedAspectRatio = true;
	camera_cmp.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	camera_cmp.Camera.SetOrthographicSize(orthoSize);

	//auto& transform = m_Handle.Transform();
	//transform.Translation.y = 1.5f;
}

void GCamera::OnGameEvent(GameEvent& e)
{
	if (!LevelManager::GetCurrentLevel().OverScreen())
		return;


	if (e.Type == GameEventType::VALUE_PLAYER_MOVING)
	{
		m_PlayerPosition = std::any_cast<glm::vec3>(e.Data);

		const auto& [width, height] = Application::Get().GetWindowSize();
		float window_pos_x, window_pos_y;

		auto& camera_cmp = m_Handle.GetComponent<CameraComponent>();
		glm::vec4 clipSpacePos = camera_cmp.Camera.GetProjection() * glm::vec4(m_PlayerPosition, 1.0);

		glm::vec3 ndcSpacePos = clipSpacePos / clipSpacePos.w;
		glm::vec2 windowSpacePos = (glm::vec2(ndcSpacePos.x + 1.0f, ndcSpacePos.y + 1.0f)) / 2.0f;


		// decompose


		//m_Handle.Transform().Translation.x = glm::clamp(windowSpacePos.x, 0.0f, 0.5f);
		//m_Handle.Transform().Translation.y = glm::clamp(windowSpacePos.x, 0.0f, 0.5f);
		//HU_INFO("1. x: {0}, y: {1}", windowSpacePos.x, windowSpacePos.y);
		//HU_INFO("2. x: {0}, y: {1}", m_PlayerPosition.x, m_PlayerPosition.y);

		//if (windowSpacePos.x > 0.5f)
		//{
		//	m_Handle.Transform().Translation.x = glm::clamp(m_PlayerPosition.x, 1.5f, m_PlayerPosition.x);
		//}
		//if (windowSpacePos.y > 0.5f)
		//{
		//	
		//
		//	m_Handle.Transform().Translation.y = m_PlayerPosition.y;
		//}
	}
}
