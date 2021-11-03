#include "PlayerControllerScript.h"

void PlayerControllerScript::OnCreate()
{
}

void PlayerControllerScript::OnDestroy()
{
}

void PlayerControllerScript::OnUpdate(Timestep ts)
{
	// Movement
	auto translation = (b2Body*)GetComponent<Rigidbody2DComponent>().RuntimeBody;

	Direction = { 0, 0 };

	if (Input::IsKeyPressed(Key::W))
	{
		Direction.y = 1;
	}
	if (Input::IsKeyPressed(Key::S))
	{
		Direction.y = -1;
	}
	if (Input::IsKeyPressed(Key::A))
	{
		Direction.x = -1;
	}
	if (Input::IsKeyPressed(Key::D))
	{
		Direction.x = 1;
	}

	Velocity.x = Direction.x * Speed;
	Velocity.y = Direction.y * Speed;

	translation->SetLinearVelocity(Velocity);
}

