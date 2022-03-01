#pragma once

#include "BomberMan/Game/Enemy.hpp"

class FastEnemy : public Enemy
{
public:
	FastEnemy(Entity& handle, Entity& grid_entity);

	void OnUpdate(Timestep& ts) override;

	void OnGameEvent(GameEvent& e) override;
protected:
	bool EnemyLogic(Timestep& ts) override;
private:
	bool m_Hit = false;
	glm::vec4 m_HitColor = glm::vec4(1.0f);
};