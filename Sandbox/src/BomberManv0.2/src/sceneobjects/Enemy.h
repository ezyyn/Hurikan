#pragma once

#include <box2d/b2_body.h>

#include <Hurikan.h>
using namespace Hurikan;

// Default init corresponds to Regular monster
struct MonsterProperties
{
	int Health = 1;
	float Speed = 1.0f;
	int Power = 0; // If a creature has its own unique ability(-Only accesible to MINIBOSS and BOSS-) 
	int IntelligencyLevel = 0;
	glm::vec3 StartPosition;
};

class Monster
{
public:
	Monster(const MonsterProperties& props);
	~Monster() = default;

	void OnSpawn();
	void OnUpdate(Timestep ts);
	void DestroyItself();
private:
	MonsterProperties m_Properties;

	glm::vec2 m_Direction = { 1.0f,1.0f };
	glm::vec2 m_Velocity = { 1.0f, 1.0f };
	float m_Speed = 1.0f;

	b2Body* m_Body;

	Entity m_Handle;
};


class EnemySpawner
{
public:
	EnemySpawner() = default;
	~EnemySpawner() = default;

	void Init();
	bool Spawn(const MonsterProperties& props);
	void OnUpdate(Timestep ts);
private:
	std::vector<Monster> m_MonsterList;
};
