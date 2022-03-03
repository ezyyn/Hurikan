#pragma once

#include "BomberMan/Game/Enemy.hpp"

class FastEnemy : public Enemy
{
public:
	FastEnemy(Entity& handle, Entity& grid_entity);

	void OnGameEvent(GameEvent& e) override;
	void OnChangeDirection(Direction& dir) override;
protected:
	bool EnemyLogic(Timestep& ts) override;
	void OnUpdateInternal(Timestep& ts) override;
};