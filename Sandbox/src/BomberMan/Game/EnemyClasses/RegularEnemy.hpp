#pragma once
#include "BomberMan/Game/Enemy.hpp"

class RegularEnemy : public Enemy
{
public:
	RegularEnemy(Entity& handle, Entity& grid_entity);

	void OnGameEvent(GameEvent& e) override;

	void OnChangeDirection(Direction& dir) override;

protected:
	bool EnemyLogic(Timestep& ts) override;

	void OnUpdateInternal(Timestep& ts) override;

private:
	// Find new random path cooldown value
	float m_FNRPC = 0.0f;
};