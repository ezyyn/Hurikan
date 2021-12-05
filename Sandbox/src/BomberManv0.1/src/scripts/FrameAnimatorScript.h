#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../core/GameComponents.h"

// TODO: investigate and propably rewrite( use registry id system? )
class FrameAnimator
{
public:
	FrameAnimator() = default;
	FrameAnimator(const FrameAnimator&) = default;
	FrameAnimator(Entity target);
	~FrameAnimator() = default;

	void Add(const AnimationBlock& ba);
	void Switch(const std::string& tag);

	AnimationBlock& GetActiveAnimation();
	AnimationBlock FrameAnimator::GetAnimationByTag(const std::string& tag);
	bool IsAnyPlaying();

	void Stop();
	bool OnUpdate(Timestep ts);
private:
	Ref<Texture2D> m_SpriteSheet = nullptr;
	glm::vec2 m_SingleSpriteSize = {0.0f, 0.0f};

	std::vector<AnimationBlock> m_Blocks;
	Entity m_TargetEntity;
};