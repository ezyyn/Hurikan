#pragma once

#include <Hurikan.h>
using namespace Hurikan;

class GameCameraController : public ScriptableEntity
{

protected:
	virtual void OnCreate() override 
	{
		//auto& translation = GetComponent<TransformComponent>().Translation;
		//translation.z = 12;

		//camera.SetPerspectiveVerticalFOV(-30.0f);
		//camera.SetPerspectiveNearClip(0.1f);
		//camera.SetPerspectiveFarClip(1000.0f);

		auto& camera = GetComponent<CameraComponent>().Camera;
		camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		camera.SetOrthographicSize(18);
	}

	virtual void OnDestroy() override 
	{
	}

	virtual void OnUpdate(Timestep ts) override 
	{
		return;
		auto& translation = GetComponent<TransformComponent>().Translation;

		static float speed = 2.0f;

		if (Input::IsKeyPressed(Key::A))
			translation.x += speed * ts;
		if (Input::IsKeyPressed(Key::D))
			translation.x -= speed * ts;
		if (Input::IsKeyPressed(Key::W))
			translation.y -= speed * ts;
		if (Input::IsKeyPressed(Key::S))
			translation.y += speed * ts;
		
	}
};

class GameCamera
{
public:
	GameCamera() = default;

	void Init(Entity& entity, uint32_t width, uint32_t height);
private:
	Entity m_GameCameraEntity;
};