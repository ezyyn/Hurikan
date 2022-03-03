#pragma once

#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/Navigation.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

#include <string>

enum class AI : unsigned short
{
	RANDOM = 0,
	FOLLOW_RANGE,
	FOLLOW
};

struct EnemyProps
{
	unsigned int Health;
	AI Intelligence;
	std::string Name;
	float Speed;
};

enum class Direction
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class Enemy : public Observable
{
public:
	Enemy(Entity& handle, Entity& grid_entity);

	virtual ~Enemy() {}

	inline const glm::vec3& GetPosition() { return m_Handle.Transform().Translation; }
	inline Entity GetLastPositionOnGrid() { return m_LastPositionOnGrid; }

	inline bool SameAs(Entity& other_handle) { return m_Handle == other_handle; }
	virtual void Follow(const std::list<Entity>& path);
	inline const EnemyProps& GetProperties() { return m_Properties; };

	void OnUpdate(Timestep & ts);
	virtual void OnGameEvent(GameEvent& e) = 0;
	virtual void OnChangeDirection(Direction& dir) = 0;
	virtual void OnHitUpdate(Timestep& ts);
protected:
	virtual void OnUpdateInternal(Timestep& ts) = 0;
private:
	virtual bool EnemyLogic(Timestep& ts) = 0;
protected:
	Entity m_LastPositionOnGrid;
	std::list<Entity> m_Path;

	Direction m_CurrentDirection;
	glm::vec3 m_PreviousPosition;

	bool m_IsRotated = false;
	bool m_Alive = true;

	bool m_Hit = false;
	glm::vec4 m_HitColor = glm::vec4(1.0f);

	EnemyProps m_Properties;

	Animator* m_Animator;
	Entity m_Handle;
};

class EnemySpawner : public Observer, public Observable
{
public:
	~EnemySpawner();

	void Init(Scene* scene);

	void Spawn(Entity& entity);

	void OnGameEvent(GameEvent& e) override;

	void OnUpdate(Timestep& ts);
private:
	std::vector<Enemy*> m_Enemies;
	std::list<Enemy*> m_ErasePool;

	Scene* g_GameScene;
};