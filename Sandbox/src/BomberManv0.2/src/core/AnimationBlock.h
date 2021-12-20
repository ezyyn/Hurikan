#pragma once

#include <Hurikan.h>
using namespace Hurikan;

struct FrameSpecification
{
	glm::vec2 Position;
	float Delay;
	bool Blank = false;
};

struct FrameData
{
	Ref<SubTexture2D> Subtexture;
	glm::vec4 Color = glm::vec4(1.0f);
	float Delay;
};

struct AnimationBlock
{
	Ref<Texture2D> SpriteSheet = nullptr;
	glm::vec2 SpriteSize = { 0.0f, 0.0f };

	std::vector<FrameData> Animation = {};
	std::string Tag = std::string();
	bool Active = false;
	bool Repeat = false;
	uint32_t Index = 0;

	AnimationBlock() = default;
	~AnimationBlock() = default;
	AnimationBlock(const AnimationBlock&) = default;
	AnimationBlock(const std::vector<FrameData>& animation, const std::string& tag) : Animation(animation), Tag(tag) {}
public:
	void Load(const std::vector<FrameSpecification>& frameSpec);
private:
	void OnUpdate(SpriteRendererComponent& src, Timestep ts);
private:
	bool FirstFrame = true;
	bool NextFrameStop = false;
	float CurrentFrameTime = 0.0f;

	friend class FrameAnimator;
};