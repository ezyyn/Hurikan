#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../scripts/FrameAnimationComponent.h"
#include "../core/TextureLoader.h"

enum class BombState
{
	Unlit, Ticking, Edge, Exploding, Done
};

class Bomb
{
public:
	Bomb(Ref<Scene> scene, Entity& entity, std::vector<Entity> explosion_spread,
		std::vector<FrameAnimationComponent::FrameData>& bomb_animation);

	Entity& GetEntity() { return m_BombEntity; }

	bool Tick(Timestep ts);
public:
	BombState m_BombState = BombState::Unlit;
private:
	Ref<Scene> m_Scene;

	std::vector<Entity> m_ExplosionSpread;

	float Duration = 2; // In seconds

	std::vector<FrameAnimationComponent::FrameData> m_ExplosionAnimation;

	std::vector<Entity> m_ExplosionEntities;
	Entity m_BombEntity;
};