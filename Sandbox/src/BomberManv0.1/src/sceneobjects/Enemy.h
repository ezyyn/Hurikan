#pragma once

#include "Player.h"

#include <Hurikan.h>
using namespace Hurikan;

// TODO: implement Intelligent enemy

enum class AIIntelligence
{
	DUMB, // Moves randomly
	COMMON, // Have some knowledge of the #player position and will be moving closely to #player
	SMART, // Have look direction and if sees the player it will follow him to the last point the player was spotted on
};

struct MonsterStats
{
	uint32_t Health;
	uint32_t Speed;
	AIIntelligence Intelligence;
};

class Enemy
{
public:
	Enemy() = default;
	void Init(Scene* scene, Player* player);
	void OnUpdate(Timestep ts);

private:
	Scene* m_GameScene;
	Player* m_Player;
	Entity m_EnemyEntity;
};