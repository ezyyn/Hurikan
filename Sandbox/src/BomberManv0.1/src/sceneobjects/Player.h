#pragma once
#include <Hurikan.h>
using namespace Hurikan;

#include "GameGrid.h"
#include "Bomb.h"

#include "../core/GameComponents.h"

struct PlayerStats
{
	PlayerStats() = default;
	PlayerStats(const PlayerStats&) = default;

	uint32_t Health = 3;
	uint32_t Power = 1; 
	uint32_t Speed = 5;

	uint32_t MaxBombCount = 5;
	uint32_t PlacedBombs = 0;
};

class Player
{
public:
	Player() = default;
	~Player() = default;
	void Init(Scene* scene, GameGrid* gamegrid);
	void OnUpdate(Timestep ts);

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);

	const std::vector<Bomb>& GetPlacedBomb()
	{
		return m_PlacedBombs;
	}
	void OnFatalHit();
public:
	// Entt helper functions TODO: maybe rewrite to be consistent
	glm::vec3& position() { return m_PlayerEntity.GetComponent<TransformComponent>().Translation; }
	PlayerStats& stats() { return m_PlayerEntity.GetComponent<PlayerStats>(); }
private:
	void PlaceBomb();
	bool CanCreateAnother();
private:
	Scene* g_GameScene;
	GameGrid* g_GameGrid;
private:
	bool m_Initialized = false;

	Ref<Texture2D> m_BombAnimationSpriteSheet;

	// Player Animations
	AnimationBlock m_PlayerLeftAnimation;
	AnimationBlock m_PlayerBreathingAnimation;
	AnimationBlock m_PlayerUpAnimation;
	AnimationBlock m_PlayerDownAnimation;
	AnimationBlock m_PlayerDead;

	// Bomb Animations
	AnimationBlock m_BombTickingAnimation;
	AnimationBlock m_SpreadExplosionAnimation;

	// TODO: SPREAD ANIMATION IMPROVE!
	AnimationBlock m_BombSpreadCenterAnimation;
	AnimationBlock m_BombSpreadWingAnimation;
	AnimationBlock m_BombSpreadEndWingAnimation;

	Entity m_PlayerEntity;

	std::vector<Bomb> m_PlacedBombs = {};

	Ref<Texture2D> SpriteSheet;
	Ref<Texture2D> SpriteSheet1;

	FrameAnimator* m_FrameAnimator = nullptr;

	friend class GameGrid;
	friend class Bomb;
};