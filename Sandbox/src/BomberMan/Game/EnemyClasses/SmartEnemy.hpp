#pragma once

#include "BomberMan/Game/Enemy.hpp"

class SmartEnemy : public Enemy
{
public:
	SmartEnemy(Entity& handle, Entity& grid_entity);

	void OnUpdateInternal(Timestep& ts) override;
	void OnChangeDirection(Direction& dir) override;
private:
	void OnGameEvent(GameEvent& e) override;
	bool EnemyLogic(Timestep& ts) override;
private:
	Entity m_PlayersParent;
	glm::vec3 m_PlayerPosition = glm::vec3(0.0f);

	bool m_InRadius = false;
	float m_FollowRadius = 5.0f;
	bool m_FollowsPlayer = false;
	bool m_RecalculatePath = true;

	float m_RecalculateTimer = 3.0f;
};