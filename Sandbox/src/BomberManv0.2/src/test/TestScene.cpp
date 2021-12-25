#include "TestScene.h"

void TestScene::Init()
{
	m_Camera = m_TestScene->CreateEntity();

	const auto& [width, height] = Application::Get().GetWindowSize();
	m_Camera.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);

	auto& camera_cmp = m_Camera.GetComponent<CameraComponent>();
	camera_cmp.FixedAspectRatio = true;
	auto& camera = camera_cmp.Camera;
	camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	//camera.SetOrthographicSize(18);

	auto& square = m_TestScene->CreateEntity();
	square.AddComponent<SpriteRendererComponent>();

	m_TestScene->OnRuntimeStart();
}

void TestScene::OnUpdate(Timestep ts)
{
	m_TestScene->OnUpdateRuntime(ts);
}

void TestScene::Shutdown()
{
	m_TestScene->OnRuntimeStop();
}
