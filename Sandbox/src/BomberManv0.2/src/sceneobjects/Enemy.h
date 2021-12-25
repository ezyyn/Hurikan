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
};

enum class EnemyMovement
{
	NONE,
	HORIZONTAL,
	VERTICAL
};

class Monster
{
public:
	Monster(const MonsterProperties& props);
	~Monster() = default;

	void OnSpawn();
	void OnUpdate(Timestep ts);
	void SetDirection(const glm::vec2& dir) noexcept { m_Direction = dir; }
	void Follow(const std::deque<glm::vec2>& path);
	void DestroyItself();

	inline TransformComponent& Transform() { return m_Handle.Transform(); }
	inline const glm::vec2& Position() { return { m_Handle.Transform().Translation.x, m_Handle.Transform().Translation.y }; }

private:
	glm::vec2 m_Direction = { 1.0f,1.0f };
	glm::vec2 m_Velocity = { 1.0f, 1.0f };
	float m_Speed = 5.0f;

	std::deque<glm::vec2> m_Path;
	std::deque<glm::vec2> m_Steering;
	bool m_IsMoving = false;

	glm::vec2 m_GetToLI;

	int m_CurrentIndex = 1;
	int m_LastIndex = -1;
	MonsterProperties m_Properties;

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
