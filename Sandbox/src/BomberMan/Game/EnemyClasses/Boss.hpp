#pragma once

#include "BomberMan/Game/Enemy.hpp"

class Boss : public Enemy
{
public:
	Boss(Scene* scene, Entity& grid_entity);

	void OnChangeDirection(Direction& dir) override;
protected:
	virtual void OnUpdateInternal(Timestep& ts) override;
	virtual void OnGameEvent(GameEvent& e) override;
	virtual bool EnemyLogic(Timestep& ts) override;
private:
	Scene* g_GameScene;

	Entity m_PlayersParent;

	bool m_RecalculatePath = true;

	bool m_Moving = false;

	bool m_CutSceneCompleted = false;

	float m_RecalculatePathTimer = 3.0f;
};