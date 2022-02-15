#pragma once

#include "BomberMan/Game/Enemy.h"

class RareEnemy : public Enemy
{
public:
	RareEnemy(Entity& handle, Entity& grid_entity);

	void OnUpdate(Timestep& ts) override;

	void OnGameEvent(GameEvent& e) override;

	const EnemyProps& GetProperties() override;

protected:
	bool EnemyLogic(Timestep& ts) override;
private:
	// Find new random path cooldown value
	float m_FNRPC = 0.0f;
	bool m_Hit = false;
	glm::vec4 m_PulseColor = glm::vec4(1.0f);

	EnemyProps m_Properties;
};