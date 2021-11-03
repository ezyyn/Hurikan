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
	void Init(Ref<Scene> scene, Ref<GameGrid> gamegrid);
	void OnUpdate(Timestep ts);

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	void PlaceBomb();
	bool CanCreateAnother();
private:
	Ref<Scene> m_GameScene;

	Ref<Texture2D> m_PlayerTexture;
	Ref<Texture2D> m_BombAnimationSpriteSheet;

	BlockAnimation m_BombTickingAnimation;
	BlockAnimation m_SpreadExplosionAnimation;

	Entity m_PlayerEntity;

	PlayerStats m_PlayerStats;

	std::vector<Bomb> m_PlacedBombs = {};

	Ref<GameGrid> m_GameGrid;
	friend class GameGrid;
};