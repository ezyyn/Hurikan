#include "PlayerScript.h"

#include "FrameAnimatorScript.h"

void PlayerScript::OnCreate()
{
}

void PlayerScript::OnDestroy()
{
}

void PlayerScript::OnUpdate(Timestep ts)
{

	// Animation
	auto& animationController = GetComponent<FrameAnimator>();
	animationController.OnUpdate(ts);

	// Movement
	auto* translation = (b2Body*)GetComponent<Rigidbody2DComponent>().RuntimeBody;

	Direction = { 0, 0 };

	static bool canBeRotated = true;
	if (Input::IsKeyPressed(Key::W))
	{
		Direction.y = 1;
		canBeRotated = true;
		GetComponent<TransformComponent>().Scale.x = glm::abs(GetComponent<TransformComponent>().Scale.x);

		if (animationController.GetActiveAnimation().Tag != "UpAnimation")
			animationController.Switch("UpAnimation");
	}
	if (Input::IsKeyPressed(Key::D))
	{
		Direction.x = 1;
		if (canBeRotated)
		{
			canBeRotated = false;
			GetComponent<TransformComponent>().Scale.x *= (-1);
		}

		if (animationController.GetActiveAnimation().Tag != "LeftAnimation")
			animationController.Switch("LeftAnimation");
	}

	if (Input::IsKeyPressed(Key::S))
	{
		Direction.y = -1;
		canBeRotated = true;
		GetComponent<TransformComponent>().Scale.x = glm::abs(GetComponent<TransformComponent>().Scale.x);


		if (animationController.GetActiveAnimation().Tag != "DownAnimation")
		{
			animationController.Switch("DownAnimation");
		}
	}
	if (Input::IsKeyPressed(Key::A))
	{
		Direction.x = -1;
		canBeRotated = true;
		GetComponent<TransformComponent>().Scale.x = glm::abs(GetComponent<TransformComponent>().Scale.x);
		
		if (animationController.GetActiveAnimation().Tag != "LeftAnimation")
			animationController.Switch("LeftAnimation");
	}

	if (Direction == b2Vec2(0, 0))
	{
		animationController.Stop();
	}

	Velocity.x = Direction.x * Speed;
	Velocity.y = Direction.y * Speed;

	translation->SetLinearVelocity(Velocity);
}

