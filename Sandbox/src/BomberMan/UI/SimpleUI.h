#pragma once

#include "BomberMan/Core/Animator.h"
#include "BomberMan/Core/Observer.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Core/Timestep.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

class SimpleUI : public Observer
{
public:
	SimpleUI() = default;
	~SimpleUI() = default;

	void Init();
	void OnUpdate(Timestep& ts);
	void Shutdown();

	void OnGameEvent(GameEvent& e) override;

private:
	void OnUpdateScore(Timestep& ts);
	void OnUpdateClock(Timestep& ts);
	// Data
	int m_ScoreOnScreen = 0;
	bool m_UpdateScore = false;
private:
	// UI sugar
	std::vector<Entity> m_HeartEntities;

	std::vector<Entity> m_TimerCountDown;
	std::vector<Entity> m_ScoreCounter;

	Scene m_SceneUI;
	Entity m_Camera;

	Entity m_TopPanel;

	int m_LastState = -1;

	Entity m_Head;

	Ref<SubTexture2D> m_IdleAnimation;
	Animator m_HeadAnimator;
};