#pragma once

#include <Hurikan.h>
using namespace Hurikan;

class GameCameraController : public ScriptableEntity
{

protected:
	virtual void OnCreate() override 
	{
		auto& translation = GetComponent<TransformComponent>().Translation;

		GetComponent<CameraComponent>().Camera.SetOrthographicSize(18);
		//translation.x = 14;
		//translation.y = 8;
	}

	virtual void OnDestroy() override 
	{
	}

	virtual void OnUpdate(Timestep ts) override 
	{
		/*
		auto& translation = GetComponent<TransformComponent>().Translation;

		float speed = 5.0f;

		if (Input::IsKeyPressed(Key::A))
			translation.x -= speed * ts;
		if (Input::IsKeyPressed(Key::D))
			translation.x += speed * ts;
		if (Input::IsKeyPressed(Key::W))
			translation.y += speed * ts;
		if (Input::IsKeyPressed(Key::S))
			translation.y -= speed * ts;
		*/
	}
private:
};

class GameCamera
{
public:
	GameCamera() = default;

	void Init(Entity& entity, uint32_t width, uint32_t height);
private:
	Entity m_GameCameraEntity;
};