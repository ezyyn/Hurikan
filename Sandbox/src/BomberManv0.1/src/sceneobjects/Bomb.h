#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../scripts/FrameAnimatorScript.h"
#include "Player.h"

enum class BombState
{
	SET = 0, TICKING, EXPLOSION
};

enum class BombType
{
	CLASSIC = 0,
	LANDMINE,
	DYNAMITE
};

struct BombProperties
{
	// Bomb
	BombState State = BombState::SET;
	BombType Type = BombType::CLASSIC;
	float ExplosionTime = 2; // in seconds 
	float Time = 0;

	// Spread Explosion
	uint32_t Reach = 1;

	std::unordered_map<std::string, std::vector<Entity>> SpreadEntities;
};

class Bomb
{
public:
	Bomb() = default;
	~Bomb() = default;

	Entity GetBombEntity()
	{
		return m_Entity;
	}

	void Init(BombProperties props, Player* player);
	void Deploy();
	bool Tick(Timestep ts);
public:
	std::vector<Entity> m_Destroyed = {};
	std::vector<Entity> m_PostAnimations = {};
private:
	void DestroyItSelf();
	void Explode();
private:
	Ref<Scene> m_GameScene;
	Player* m_Player;

	BombProperties m_Properties;

	std::vector<Entity> m_LeftWing;
	std::vector<Entity> m_UpWing;
	std::vector<Entity> m_RightWing;
	std::vector<Entity> m_DownWing;

	Entity m_Entity;

	bool m_ChainExplosion = false;

	// Maybe add to entity itself:
	const glm::vec3& translation() 
	{
		return m_Entity.GetComponent<TransformComponent>().Translation;
	}
private:
	void initSpread(Entity entity);
	void WingInitialization(int index, std::vector<Entity> wing)
};