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
		for (auto& frame : GetComponent<AnimationComponent>().Animation)
		{
			Animation.AddFrame(frame.Texture, frame.Delay);
		}
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
			GetComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f, 1.0f, 0.0f };
			return;
		}
		GetComponent<SpriteRendererComponent>().Texture = currentFrame->Texture;
		GetComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f, 1.0f, 1.0f };

		Animation.Update(ts);
	}

	FrameAnimation2D Animation;
};

class Bomb
{
public:
	Bomb(Entity& entity, FrameAnimation2D bomb_anim);

	Entity& GetEntity() { return m_BombEntity; }

	bool Exploded = false;
private:
	Entity m_BombEntity;
};