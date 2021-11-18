#pragma once
#include <Hurikan.h>
using namespace Hurikan;

#include "GameGrid.h"
#include "Bomb.h"

#include "../core/GameComponents.h"

struct PlayerStats
{
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
private:
	void PlaceBomb();
	bool CanCreateAnother();
private:
	Scene* m_GameScene;

	Ref<Texture2D> m_PlayerTexture;
	Ref<Texture2D> m_BombAnimationSpriteSheet;

	// Player Animations
	AnimationBlock m_PlayerLeftAnimation;
	AnimationBlock m_PlayerBreathingAnimation;
	AnimationBlock m_PlayerUpAnimation;
	AnimationBlock m_PlayerDownAnimation;

	// Bomb Animations
	AnimationBlock m_BombTickingAnimation;
	AnimationBlock m_SpreadExplosionAnimation;

	// TODO: SPREAD ANIMATION IMPROVE!
	AnimationBlock m_BombSpreadCenterAnimation;
	AnimationBlock m_BombSpreadWingAnimation;
	AnimationBlock m_BombSpreadEndWingAnimation;

	Entity m_PlayerEntity;

	PlayerStats m_PlayerStats;

	std::vector<Bomb> m_PlacedBombs = {};
	std::vector<Entity> m_DestroyedBoxes = {};

	GameGrid* m_GameGrid;
	friend class GameGrid;
	friend class Bomb;
};