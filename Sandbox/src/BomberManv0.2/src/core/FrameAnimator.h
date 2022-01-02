#pragma once

#include "../core/AnimationBlock.h"

class FrameAnimator
{
public:
	FrameAnimator() = default;
	FrameAnimator(const FrameAnimator&) = default;
	~FrameAnimator();

	void Add(const AnimationBlock& ba);
	void Play(const std::string& tag);
	void Pause();
	void Stop();

	void SetTarget(Entity target);

	bool IsActive(const std::string& tag);

	AnimationBlock* GetActiveAnimation();

	bool IsAnyPlaying();
	bool OnUpdate(Timestep ts);
private:
	Ref<Texture2D> m_SpriteSheet = nullptr;
	glm::vec2 m_SingleSpriteSize = {0.0f, 0.0f};

	AnimationBlock* m_ActiveAnimation = nullptr;

	SpriteRendererComponent* m_TargetSRC = nullptr;

	std::vector<AnimationBlock> m_Blocks;
	Entity m_TargetEntity;
};