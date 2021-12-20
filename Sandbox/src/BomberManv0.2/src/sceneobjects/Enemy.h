#pragma once

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

	Entity m_Handle;
private:
	Scene* g_GameScene = nullptr;
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
