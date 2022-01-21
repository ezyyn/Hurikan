#pragma once

#include "BomberMan/Core/Observer.h"
#include "BomberMan/Core/LevelManager.h"

#include <Hurikan/Core/Ref.h>
#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

class Hurikan::Scene;
using namespace Hurikan;

enum class EntityType
{
	PLAYER = 0,
	BOMB,
	EMPTY,
	WALL,
	LOOT_WALL,
	ENEMY
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
	void Create(Hurikan::Scene& scene);

	static inline Entity& Get(unsigned int y, unsigned int x) { return m_Grid[y * LevelManager::GetCurrentLevel().Width + x]; }

	inline const int GetLevelHeight() const { return m_CurrentLevel.Height; }
	inline const int GetLevelWidth()  const { return m_CurrentLevel.Width; }

	virtual void OnGameEvent(GameEvent& e) override;
private:
	static Scope<Entity[]> m_Grid;
	std::vector<Entity> m_DestroyableWalls;

	Entity m_PlayerGridPosition;

	Level m_CurrentLevel;
};