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

class AnimationBlock
{
public:
	AnimationBlock() = default;
	AnimationBlock(const AnimationBlock& other)
	{
		this->Active = other.Active;
		this->Animation = other.Animation;
		this->ID = other.ID;
		this->Repeat = other.Repeat;
		this->SpriteSize = other.SpriteSize;
		this->Tag = other.Tag;
		this->SpriteSheet = other.SpriteSheet;
	}
	~AnimationBlock() = default;

	void Load(const std::vector<FrameSpecification>& frameSpec);
public:
	Ref<Texture2D> SpriteSheet = nullptr;
	glm::vec2 SpriteSize = { 0.0f, 0.0f };

	std::vector<FrameData> Animation;
	std::string Tag = std::string();
	bool Active = false;
	bool Repeat = false;
	bool NextFrameStop = false;
	size_t Index = 0;
private:
	void OnUpdate(SpriteRendererComponent* src, Timestep ts);
private:
	float CurrentFrameTime = 0.0f;
	int ID = -1;

	friend class FrameAnimator;
};