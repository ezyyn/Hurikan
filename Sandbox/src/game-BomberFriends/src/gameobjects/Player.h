#pragma once
#include "Bomb.h"
#include "GameGrid.h"

#include <box2d/b2_body.h>
#include <Hurikan.h>
using namespace Hurikan;

class Player
{
public:
	Player() = default;

	void Init(Ref<Scene> scene, Ref<GameGrid> gamegrid);

	Entity& GetEntity() { return m_PlayerEntity; }

	void OnUpdate(Timestep ts);
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);

	// TEMP:
	int count = 5;
private:
	Ref<Scene> m_Scene;
	Entity m_PlayerEntity;

	Ref<GameGrid> m_GameGrid;

	// TEMP:
	std::vector<FrameAnimationComponent::FrameData> m_BombAnimation;
	std::vector<FrameAnimationComponent::FrameData> m_ExplosionAnimation;
	std::vector<FrameAnimationComponent::FrameData> m_PlayerAnimations;

	std::vector<Bomb> m_PlacedBombs;

	friend class PlayerController;
};