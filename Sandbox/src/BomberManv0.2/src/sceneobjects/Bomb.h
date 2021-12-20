#pragma once

#include "../sceneobjects/Grid.h"
#include "../core/Level.h"
#include "../core/FrameAnimator.h"

#include <Hurikan.h>
using namespace Hurikan;

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
	glm::vec2 StartPosition = {};
	BombState State = BombState::SET;
	BombType Type = BombType::CLASSIC;
	float ExplosionTime = 2; // in seconds 
	float Time = 0;

	Level CurrentLevel = {};

	// Spread Explosion
	int Reach = 2;

	std::vector<std::vector<Entity>> SpreadEntities = {};

};

class Bomb
{
public:
	Bomb(const BombProperties& bombprops);
	~Bomb() = default;

	Entity GetBombEntity() { return m_Handle; }

	void Deploy();
	bool Tick(Timestep ts);

	BombState GetState() { return m_Properties.State; }

	const glm::vec3& Position() { return m_Handle.Transform().Translation; }
private:
	void WingInit();

	void DestroyItSelf();
	void Explode() { m_ChainExplosion = true; }
	bool WingInitialization(int y, int distance, bool condition, float rotationZ, std::vector<Entity>& wing);
	void AddAnimations(std::vector<Entity>& wing);
private:
	BombProperties m_Properties;

	std::vector<Entity> m_LeftWing = {};
	std::vector<Entity> m_UpWing = {};
	std::vector<Entity> m_RightWing = {};
	std::vector<Entity> m_DownWing = {};

	Entity m_Handle;
	GridNode* m_BombPlaced;
	bool m_ChainExplosion = false;
	bool m_AnmCmplt = false;
private:
	FrameAnimator m_BombAnimator;

	AnimationBlock 
		m_BombSpreadCenterAnimation,
		m_BombSpreadMiddleAnimation,
		m_BombSpreadEndAnimation;

	friend class BombManager;
};

class BombManager
{
public:
	BombManager() = default;
	~BombManager() = default;

	void Init(const Level& level);
	bool PlaceBomb(const BombProperties& bomb);
	void OnUpdate(Timestep ts);
private:
	Level m_CurrentLevel;
private:
	GridNode** g_GameGrid = nullptr;
};
