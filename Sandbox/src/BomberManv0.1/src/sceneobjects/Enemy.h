#pragma once

#include "Player.h"

#include <Hurikan.h>
using namespace Hurikan;
/* 
	NICE CONCEPT TOO COMPLICATED
enum MonsterRunes
{
	// Intelligency
	INTELLIGENCY_DUMB = BIT(0), // Moves randomly
	INTELLIGENCY_SMART = BIT(1), // Have some knowledge of the #player position and will be moving closely to #player
	INTELLIGENCY_GENIUS = BIT(2), // Have look direction and if sees the player it will follow him to the last point the player was spotted on

	// Creature level
	CREATURELEVEL_REGULAR = BIT(3), // Everyday monster u know
	CREATURELEVEL_MINIBOSS = BIT(4), // Every level has its own miniboss
	CREATURELEVEL_BOSS = BIT(5), // Every 10 levels there is always a unique boss

	// Buffs & Debuffs
	BUFF_FAST = BIT(6),
	BUFF_POWER = BIT(7), 
	BUFF_HEALTH = BIT(8),

	// STATS
	STATS_SPEED5 = BIT(9),
};


bool IsInCategory(int target, int other)
{
	return target & other;
}
*/

// Default initilatization coresponds to Regular monster
struct MonsterStats
{
	MonsterStats() = default;
	MonsterStats(const MonsterStats&) = default;

	uint32_t Health = 1;
	uint32_t Speed = 4;
	uint32_t Power = 0; // If a creature has its own unique ability(-Only accesible to MINIBOSS and BOSS-) 
	uint32_t IntelligencyLevel = 0; 
};

struct Monster
{
	Entity MonsterEntity;
	inline const glm::vec3& position()
	{
		return MonsterEntity.GetComponent<TransformComponent>().Translation;
	}
};

class EnemySpawner
{
public:
	EnemySpawner() = default;
	~EnemySpawner() = default;
	void Init(Scene* scene, GameGrid* gamegrid, Player* player);

	void Spawn(const MonsterStats& stats);
	void OnUpdate(Timestep ts);
private:
	std::vector<Monster> m_MonsterList;
private:
	// Global variables
	GameGrid* g_GameGrid;
	Scene* g_GameScene;
	Player* g_Player;
};