#pragma once

#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/Core/Utils.hpp"

#include <Hurikan/Core/Ref.h>
#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;


enum class EntityType
{
	PLAYER = 0,
	BOMB,
	EMPTY,
	WALL,
	BREAKABLE_WALL,
	ENEMY_REGULAR,
	ENEMY_FAST,
	ENEMY_SMART,
	ENEMY_BOSS,
};

enum class Loot
{
	EXIT = 0, // requires killing all of the monsters
	BOMB_UPGRADE_POWER,
	BOMB_UPGRADE_COUNT,
	SPEED_UPGRADE,
	HEALTH_POINT,
	KEY
};

struct LootComponent
{
	Loot Type;
	Entity LootHandle;
	bool Obtainable = false;

	LootComponent() = default;
	LootComponent(const LootComponent&) = default;
	LootComponent(const Loot& type) : Type(type) {}
};

struct EntityTypeComponent
{
	EntityType Type;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(const EntityType& type) : Type(type) {}
};

struct GridNodeComponent
{
	GridNodeComponent() = default;
	GridNodeComponent(const GridNodeComponent&) = default;

	bool Visited = false;
	bool Obstacle = false;
	float GlobalGoal = 0.0f;
	float LocalGoal = 0.0f;

	glm::ivec2 Index = {};

	std::vector<Entity> Neighbours;
	Entity Parent;
};

class Grid : public Observer, public Observable
{ 
public:
	~Grid();

	void Create(Scene* const scene);

	static Entity& Get(unsigned int y, unsigned int x);

	const int GetLevelHeight() const; 
	const int GetLevelWidth() const;  

	static void ClearNodes();

	virtual void OnGameEvent(GameEvent& e) override;
	void OnUpdate(Timestep& ts);
private:
	static Scope<Entity[]> m_Grid;

	std::vector<Entity> m_AnimationQueue;

	bool m_KeyObtained = false;

	Entity 
		m_PlayerGridPosition, 
		m_KeyGridEntity;
	Scene* g_GameScene;
};