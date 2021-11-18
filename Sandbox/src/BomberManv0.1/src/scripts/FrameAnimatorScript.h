#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../core/GameComponents.h"

class FrameAnimator
{
public:
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
	AnimationBlock m_EmptyBA;
	Entity m_TargetEntity;
};