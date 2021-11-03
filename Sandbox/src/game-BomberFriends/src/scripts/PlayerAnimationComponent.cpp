#include "PlayerAnimationComponent.h"

void PlayerAnimationComponent::OnCreate()
{
	FrameAnimationComponent::OnCreate();
	FrameAnimationComponent::Play();
}

void PlayerAnimationComponent::OnDestroy()
{
	FrameAnimationComponent::OnDestroy();
}

static bool Loaded = false;

void PlayerAnimationComponent::OnUpdate(Timestep ts)
{
	FrameAnimationComponent::Stop();
	FrameAnimationComponent::SetAnimationIndex(0);

	if (Input::IsKeyPressed(Key::W))
	{
	}
	if (Input::IsKeyPressed(Key::A))
	{
	}
	if (Input::IsKeyPressed(Key::S))
	{
		FrameAnimationComponent::Play();
		FrameAnimationComponent::SelectPartOfAnimation(1, 2);
	}
	if (Input::IsKeyPressed(Key::D))
	{
	}

	// This changes the frame
	FrameAnimationComponent::OnUpdate(ts);
}
