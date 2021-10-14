#include "GameCamera.h"

void GameCamera::Init(Entity& entity, uint32_t width, uint32_t height)
{
	m_GameCameraEntity = entity;

	m_GameCameraEntity.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);
	m_GameCameraEntity.AddComponent<NativeScriptComponent>().Bind<GameCameraController>();
}
