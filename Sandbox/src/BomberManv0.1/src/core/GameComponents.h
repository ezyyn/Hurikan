#pragma once

#include <Hurikan.h>
using namespace Hurikan;

enum class EntityType
{
	CAMERA, PLAYER, BACKGROUND,
	BOMB, BOMB_SPREAD_EXPLOSION,
	TILE_EMPTY, TILE_WALL, TILE_BOX, TILE_PSP/* PlayerStartupPosition */ , TILE_BOMB_OCCUPIED
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

// Not a component
struct BlockAnimation
{
	Ref<Texture2D> SpriteSheet;
	glm::vec2 SpriteSize;

	std::vector<FrameData> Animation;
	std::string Tag;
	bool Active = false;
	bool Repeat = false;
	uint32_t Index = 0;

	BlockAnimation Reset()
	{
		Index = 0;
		Active = false;
		return *this;
	}

	BlockAnimation() = default;
	BlockAnimation(const BlockAnimation&) = default;
	BlockAnimation(const std::vector<FrameData>& animation, const std::string& tag) : Animation(animation), Tag(tag) {}
private:
	void OnUpdate(SpriteRendererComponent& src, Timestep ts)
	{
		if (Animation.empty())
			return;

		if (Index == Animation.size() - 1)
		{
			if (Repeat)
				Index = 0;
			else
				Active = false;
		}

		if (!Active)
			return;

		m_CurrentFrameTime += ts;

		if (m_CurrentFrameTime >= Animation[Index].Delay / 1000) // <- Maybe change
		{
			m_CurrentFrameTime = 0.0f;
			Index++;
			//Index = (Index + 1) % Animation.size();
		}

		src.SubTexture = Animation[Index].Subtexture;
	}
private:
	float m_CurrentFrameTime = 0.0f;

	friend class FrameAnimator;
};

struct AnimationComponent
{
	std::vector<BlockAnimation> Blocks;

	AnimationComponent() = default;
	AnimationComponent(const AnimationComponent&) = default;
	AnimationComponent(const std::vector<BlockAnimation>& blocks) : Blocks(blocks) {}
};
