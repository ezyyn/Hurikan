#pragma once

#include "../animations/FrameAnimator.h"
#include "../core/Level.h"

#include <Hurikan.h>
using namespace Hurikan;

// forward declarations
struct GridNode;

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

struct BombProperties
{
	glm::vec2 StartPosition = {};
	BombState State = BombState::SET;
	BombType Type = BombType::CLASSIC;
	float ExplosionTime = 2; // in seconds 
	float Time = 0;
	Level CurrentLevel = {};
	int Reach = 2;
};

class Bomb
{
public:
	Bomb() = default;
	Bomb(const Bomb&) = default;
	Bomb(const BombProperties& bombprops);
	~Bomb();

	Entity GetBombEntity() { return m_Handle; }

	bool Tick(Timestep ts);

	BombState GetState() { return m_Properties.State; }

	const glm::vec3& Position() { return m_Handle.Transform().Translation; }
private:
	void WingInit();

	bool SingleWingInit(int index, GridNode* node, float rotation);

	void DestroyItSelf();
	void Explode();
private:
	BombProperties m_Properties;

	std::vector<Entity> m_SpreadEntities = {};

	Entity m_Handle;
	GridNode* m_BombPlaced = nullptr;
	bool m_ChainExplosion = false;
	bool m_AnmCmplt = false;

	// Exp
	AnimationDetails a, b, c;

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
public:
	static std::list<Bomb> s_PlacedBombs;
private:
	Level m_CurrentLevel;
};