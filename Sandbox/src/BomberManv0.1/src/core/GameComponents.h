#pragma once

#include <Hurikan.h>
using namespace Hurikan;
enum class EntityType

{
	CAMERA, PLAYER, BACKGROUND,
	BOMB, BOMB_SPREAD_EXPLOSION,
	TILE_EMPTY, TILE_TO_BE_EMPTY,
	TILE_WALL, TILE_BOX, TILE_PSP, TILE_MSP, TILE_OCCUPIED_BOMB,
	ENEMY
};

struct EntityTypeComponent
{
	EntityType Type;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(const EntityType& type) : Type(type) {}
};

struct FrameData
{
	Ref<SubTexture2D> Subtexture;
	float Delay;
};

// SubComponent
struct AnimationBlock
{
	Ref<Texture2D> SpriteSheet = nullptr;
	glm::vec2 SpriteSize = {0.0f, 0.0f};

	std::vector<FrameData> Animation = {};
	std::string Tag;
	bool Active = false;
	bool Repeat = false;
	uint32_t Index = 0;

	AnimationBlock Reset()
	{
		Index = 0;
		Active = false;
		return *this;
	}

	AnimationBlock() = default;
	~AnimationBlock() = default;
	AnimationBlock(const AnimationBlock&) = default;
	AnimationBlock(const std::vector<FrameData>& animation, const std::string& tag) : Animation(animation), Tag(tag) {}
private:
	bool nextFrameStop = false;

	void OnUpdate(SpriteRendererComponent& src, Timestep ts)
	{
		if (Animation.empty())
			return;

		if (!Active)
			return;

		m_CurrentFrameTime += ts;

		if (m_CurrentFrameTime >= Animation[Index].Delay / 1000) // <- definetely change
		{
			m_CurrentFrameTime = 0.0f;
			if (nextFrameStop)
			{
				nextFrameStop = false;
				Index = 0;
			}
			else {
				Index++;
			}
			//Index = (Index + 1) % Animation.size();
		}

		if (Index == Animation.size() - 1)
		{
			if (Repeat)
			{
				nextFrameStop = true;
			}
			else
			{
				Index = 0;
				Active = false;
				return;
			}
		}

		src.SubTexture = Animation[Index].Subtexture;
	}
private:
	float m_CurrentFrameTime = 0.0f;

	friend class FrameAnimator;
};

