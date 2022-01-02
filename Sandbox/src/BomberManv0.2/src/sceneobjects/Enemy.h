#pragma once

#include "../core/Pathing.h"

#include <box2d/b2_body.h>

#include <Hurikan.h>
using namespace Hurikan;

// Default init corresponds to Regular monster
struct MonsterProperties
{
	int Health = 1;
	float Speed = 2.0f;
	int Power = 0; // If a creature has its own unique ability(-Only accesible to MINIBOSS and BOSS-) 
	int IntelligencyLevel = 0;
	glm::vec3 StartPosition;

	float AttackRadius = 5.0f;
};

class Monster
{
public:
	Monster(const MonsterProperties& props);
	~Monster() = default;

	void OnUpdate(Timestep ts);
	void Follow(const std::deque<glm::vec2>& path);
	void DestroyItself();

	inline TransformComponent& Transform() { return m_Handle.Transform(); }
	inline const glm::vec2 Position() { return { m_Handle.Transform().Translation.x, m_Handle.Transform().Translation.y }; }
private:
	void OnSpawn();
private:
	MonsterProperties m_Properties;

	std::deque<glm::vec2> m_Path;
	bool m_IsMoving = false;

	bool m_Vertical = false;
	bool m_Horizontal = false;

	unsigned int m_CurrentIndex = 1;

	Entity m_Handle;

	friend class EnemySpawner;
};

class EnemySpawner
{
public:
	EnemySpawner() = default;
	~EnemySpawner() = default;

	void Init();
	bool Spawn(const MonsterProperties& props);
	void UpdatePaths();
	void OnUpdate(Timestep ts);
	inline std::vector<Monster>& GetMonsters() noexcept { return m_MonsterList; }
private:
	std::vector<Monster> m_MonsterList;
	PathFinder m_PathFinder;
};
