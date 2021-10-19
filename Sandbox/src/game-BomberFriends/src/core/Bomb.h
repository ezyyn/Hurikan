#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "FrameAnimation2D.h"
#include "TextureLoader.h"

class BombScript : public ScriptableEntity
{
protected:
	virtual void OnCreate() override
	{
		auto& animation_c = GetComponent<AnimationComponent>();

		for (auto& frame : animation_c.Animation)
		{
			Animation.AddFrame(frame.Texture, frame.Delay);
		}
		animation_c.IsPlaying = true;
		Animation.Play();
	}

	virtual void OnDestroy() override
	{
	}

	virtual void OnUpdate(Timestep ts) override
	{
		Frame* currentFrame = Animation.CurrentFrame();
		if (currentFrame == nullptr)
		{
			auto& animation_c = GetComponent<AnimationComponent>();

			GetComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f, 1.0f, 0.0f };
			animation_c.IsPlaying = false;
			return;
		}
		GetComponent<SpriteRendererComponent>().Texture = currentFrame->Texture;
		GetComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f, 1.0f, 1.0f };

		Animation.Update(ts);
	}
private:
	FrameAnimation2D Animation;
};

class Bomb
{
public:
	Bomb(Entity& entity, FrameAnimation2D bomb_anim);

	Entity& GetEntity() { return m_BombEntity; }
private:

	float Duration = 2; // In seconds
	Entity m_BombEntity;
};