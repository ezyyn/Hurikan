#include "GCamera.h"

#include <Hurikan/Core/Application.h>
#include <Hurikan/Scene/Components.h>

void GCamera::Create(Entity& e)
{
	constexpr float orthoSize = 18;

	const auto& [width, height] = Application::Get().GetWindowSize();

	auto& camera = e;
	auto& camera_cmp = camera.AddComponent<CameraComponent>();
	camera_cmp.Camera.SetViewportSize(width, height);;
	camera_cmp.FixedAspectRatio = true;
	camera_cmp.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	camera_cmp.Camera.SetOrthographicSize(orthoSize);
}
