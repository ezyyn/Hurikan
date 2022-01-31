#pragma once

#include "BomberMan/Core/Observer.h"

#include <string>
#include <queue>

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

enum class EnemyType : unsigned short
{
	REGULAR = 0,
	RARE,
	BOSS
};

enum class AI : unsigned short
{
	DUMB = 0,
	SMART,
	GENIUS
};

struct EnemyProps
{
	EnemyProps() = delete;

	EnemyProps(Entity& GRID_ENTITY, const EnemyType& type)
		: Type(type), SpawnPoint(GRID_ENTITY)
	{
		switch (type)
		{
		case EnemyType::REGULAR:
			Health = 1;
			Intelligence = AI::DUMB;
			Name = "Bongo";
			break;
		case EnemyType::RARE:
			Health = 2;
			Intelligence = AI::SMART;
			Name = "Bingus";
			break;
		case EnemyType::BOSS:
			Health = 5;
			Intelligence = AI::GENIUS;
			Name = "Bap";
			break;
		default:
			HU_CORE_ASSERT(false, "");
			break;
		}
	}

	unsigned int Health;
	AI Intelligence;
	std::string Name;
	EnemyType Type = EnemyType::REGULAR;
	Entity SpawnPoint;
};

class Enemy : public Observable
{
	enum class Direction
	{
		UP = 0, 
		DOWN, 
		LEFT, 
		RIGHT,
		NONE
	};

public:
	Enemy(Entity& handle, const EnemyProps& props);

	void Follow(const std::list<Entity>& path);
	inline Entity& LastPosition() 
	{ 
		return m_LastPos;
	}
	void OnUpdate(Timestep& ts);

	inline const EnemyProps& GetProperties() { return m_Props; }
private:
	EnemyProps m_Props;

	std::list<Entity> m_Path;
	Direction m_CurrentDirection;

	Entity m_LastPos;
	Entity m_Handle;

	float prev_x = 0;
	float prev_y = 0;

	bool rotated = false;
};

class EnemySpawner : public Observer, public Observable
{
public:
	void Init(Scene* scene);

	void Spawn(EnemyProps& props);

	void OnGameEvent(GameEvent& e) override;

	void OnUpdate(Timestep& ts);
private:
	void UpdatePaths(GameEventType& type);
private:
	std::vector<Enemy> m_Enemies;

	Entity m_PlayedGridPosition;

	Scene* g_GameScene;
};