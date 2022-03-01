#pragma once

#include "BomberMan/Core/Observer.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

enum class BombState : unsigned short
{
	SET = 0, TICKING, EDGE, EXPLOSION, DONE
};

enum class BombType
{
	CLASSIC = 0,
	LANDMINE,
	DYNAMITE
};

struct BombProps
{
	BombType Type = BombType::CLASSIC;
	float ExplosionTime = 2; // in seconds 
	int Reach = 2;
private:
	Entity GridEntity;
	BombState State = BombState::SET;
	float Time = 0;

	friend class BombManager;
	friend class Bomb;
};

class Bomb : public Observable
{
public:
	Bomb(const BombProps& props, Scene* scene);

	bool OnTick(Timestep& ts);
public:
	inline const glm::vec3& Position() { return m_Handle.Transform().Translation; }
	inline const BombState& GetState() { return m_Properties.State; }
	inline Entity& GetParent() { return m_Parent; }
	inline bool PhysicsEnabled() { return m_PhysicsEnabled; }
	void EnablePhysics();
private:
	void Explode();
	void Expand();
	bool WingExpand(int index, Entity& entity, float rotation);
	void DestroyItSelf();

	inline bool operator==(Bomb& other) { return m_Handle == other.m_Handle; }
	inline bool operator!=(Bomb& other) { return !(*this == other); }
private:
	std::list<Entity> m_SpreadEntities;
	bool m_ChainExplosion = false;
	int m_AnimationCompletedCount = 0;
	bool m_PhysicsEnabled = false;

	BombProps m_Properties;
	Entity m_Parent; 
	Entity m_Handle;
private:
	Scene* g_GameScene;
};

class BombManager : public Observer, public Observable
{
public:
	~BombManager();

	void Init(Scene* g_GameScene);
	void OnGameEvent(GameEvent& e) override;
	void OnUpdate(Timestep& ts);
private:
	bool PlaceBomb(BombProps& props);
private:
	Scene* g_GameScene;
	Entity m_PlayerGrid; // Entity behind the player 
};