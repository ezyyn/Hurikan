#include "GameCamera.h"

void GameCamera::Init(Ref<Scene> scene, uint32_t width, uint32_t height)
{
	m_GameCameraEntity = scene->CreateEntity("GameCamera");

	m_GameCameraEntity.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);
	m_GameCameraEntity.AddComponent<NativeScriptComponent>().Bind<GameCameraController>();
}
