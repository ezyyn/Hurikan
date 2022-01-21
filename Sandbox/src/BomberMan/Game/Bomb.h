#pragma once

#include "BomberMan/Core/Observer.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

enum class BombState
{
	SET = 0, TICKING, EXPLOSION, DONE
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
	//Level CurrentLevel = {};

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
private:
	void Explode();
	void Expand();
	bool WingExpand(int index, Entity& entity, float rotation);
private:
	std::list<Entity> m_SpreadEntities;
	bool m_ChainExplosion = false;
	bool amn_cmlpt = true;

	BombProps m_Properties;
	Entity m_OnTopOfEntity; // Entity from grid array that the object is on top of
	Entity m_Handle;
private:
	Scene* g_GameScene;
};

class BombManager : public Observer, public Observable
{
public:
	void Init(Scene* g_GameScene);
	bool PlaceBomb(BombProps& props);
	void OnGameEvent(GameEvent& e) override;
	void OnUpdate(Timestep& ts);
private:
	Scene* g_GameScene;
	Entity m_PlayerGrid; // Entity behind the player right now
};