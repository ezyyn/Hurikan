#include "GameCamera.h"

void GameCamera::Init(Entity& entity, uint32_t width, uint32_t height)
{
	//m_GameCameraEntity = entity;

	entity.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);
	entity.AddComponent<NativeScriptComponent>().Bind<GameCameraController>();
}
