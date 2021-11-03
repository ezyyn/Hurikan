#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../scripts/FrameAnimatorScript.h"

enum class BombState
{
	UNLIT = 0, TICKING, EDGE, EXPLODING, DONE
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
	Entity BombEntity;
	BombState State = BombState::UNLIT;
	BombType Type = BombType::CLASSIC;
	float Length = 2;

	// Spread Explosion
	uint32_t Reach = 1;

	std::vector<Entity> SpreadEntities;
	
	//Entity LeftEntity,
	//	   UpEntity,
	//	   RightEntity,
	//	   DownEntity; // Clockwise
};

class Bomb
{
public:
	Bomb(BombProperties props);

	Entity GetBombEntity()
	{
		return m_Properties.BombEntity;
	}
	
	std::vector<Entity>& GetSpreadEntities()
	{
		return m_Properties.SpreadEntities;
	}

	bool Tick(Timestep ts);
private:
	FrameAnimator* m_FrameAnimator;

	BombProperties m_Properties;

	std::vector<Entity> m_SpreadExplosion = {};
};