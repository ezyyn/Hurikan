#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include <box2d/b2_body.h>

#include "Bomb.h"

class PlayerController : public ScriptableEntity
{
private:
	virtual void PlayerController::OnCreate() override
	{
	}

	virtual void PlayerController::OnDestroy() override
	{
	}

	virtual void PlayerController::OnUpdate(Timestep ts) override
	{
		// Movement
		auto translation = (b2Body*)GetComponent<Rigidbody2DComponent>().RuntimeBody;
		
		Direction = { 0, 0 };

		if (Input::IsKeyPressed(Key::W))
			Direction.y = 1;

		if (Input::IsKeyPressed(Key::S))
			Direction.y = -1;

		if (Input::IsKeyPressed(Key::A))
			Direction.x = -1;

		if (Input::IsKeyPressed(Key::D))
			Direction.x = 1;

		Velocity.x = Direction.x * Speed;
		Velocity.y = Direction.y * Speed;
		
		translation->SetLinearVelocity(Velocity);
	}
private:
	b2Vec2 Velocity = { 0.0f,0.0f };
	b2Vec2 Direction = { 0.0f,0.0f };
	float Speed = 5.0f;
};

class Player
{
public:
	Player() = default;

	void Init(Ref<Scene> scene);

	Entity& GetEntity() { return m_PlayerEntity; }

	void OnUpdate(Timestep& ts);

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);

	// TEMP:
	int count = 5;

private:
	Ref<Scene> m_Scene;

	Entity m_PlayerEntity;
	FrameAnimation2D m_BombAnimation;

	friend class PlayerController;
};