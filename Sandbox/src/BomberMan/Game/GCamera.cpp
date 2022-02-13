#include "GCamera.h"

#include "BomberMan/Core/SaveManager.h"
#include "BomberMan/Game/Grid.h"

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
}

void GCamera::OnGameEvent(GameEvent& e)
{
	if (!SaveManager::GetCurrentLevel().OverScreen())
		return;

	if (e.Type == GameEventType::VALUE_PLAYER_MOVING || e.Type == GameEventType::VALUE_PLAYER_START_POS)
	{
		m_PlayerPosition = std::any_cast<glm::vec3>(e.Data);

		m_Handle.Transform().Translation.x = glm::clamp(m_PlayerPosition.x, 0.0f,
			Utils::RoundUp((SaveManager::GetCurrentLevel().Width - MIN_LEVEL_WIDTH)));

		m_Handle.Transform().Translation.y = glm::clamp(m_PlayerPosition.y, Utils::RoundUp((SaveManager::GetCurrentLevel().Height - MIN_LEVEL_HEIGHT) / -2.0f), 
											Utils::RoundUp((SaveManager::GetCurrentLevel().Height - MIN_LEVEL_HEIGHT) / 2.0f));
	}
}
