#pragma once

#include "BomberMan/Game/Enemy.hpp"

class SmartEnemy : public Enemy
{
public:
	SmartEnemy(Entity& handle, Entity& grid_entity);

	void OnUpdate(Timestep& ts) override;

	void OnGameEvent(GameEvent& e) override;

private:
	bool EnemyLogic(Timestep& ts) override;
private:
	bool m_Hit = false;
	glm::vec4 m_HitColor = glm::vec4(1.0f);

	Entity m_PlayersParent;
	glm::vec3 m_PlayerPosition = glm::vec3(0.0f);

	bool m_InRadius = false;
	float m_FollowRadius = 5.0f;
	bool m_FollowsPlayer = false;
	bool m_RecalculatePath = true;

	float m_RecalculateTimer = 3.0f;
};