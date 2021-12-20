#pragma once

#include "../core/AnimationBlock.h"

class FrameAnimator
{
public:
	FrameAnimator() = default;
	~FrameAnimator() = default;

	void Add(const AnimationBlock& ba);
	void Switch(const std::string& tag);

	void SetTarget(Entity target) 
	{
		m_TargetEntity = target; 
	}

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