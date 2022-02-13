#pragma once

#include "BomberMan/Core/Animator.h"
#include "BomberMan/Core/Observer.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Core/Timestep.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;


class SimpleUI : public Observer, public Observable
{
public:
	enum class PauseMenuOption
	{
		NONE,
		SETTINGS,
		MAIN_MENU,
		EXIT
	};

	SimpleUI() = default;
	~SimpleUI() = default;

	void Init();
	void OnUpdate(Timestep& ts, bool paused);
	void Shutdown();

	void OnGameEvent(GameEvent& e) override;

	void DisplayPauseMenu();
	void HidePauseMenu();
private:
	void OnUpdateScore(Timestep& ts);
	void OnUpdateClock(Timestep& ts);
	// Data
	int m_ScoreOnScreen = 0;
	bool m_UpdateScore = false;
private:
	// UI sugar
	Entity m_HeartEntities[3];

	std::vector<Entity> m_TimerCountDown;
	std::vector<Entity> m_ScoreCounter;

	Entity m_Camera;

	Entity m_TopPanel;

	int m_LastState = -1;
	float m_ClockTimer = 1.0f;
	float m_ScoreTimer = 0.0f;

	unsigned short m_LastKey = 0;
	PauseMenuOption 
		m_SelectedOption, 
		m_ConfirmedOption;

	Entity m_Head;

	Entity 
		m_PauseMenu,
		m_PauseMenuArrowHead;

	Ref<SubTexture2D> m_IdleAnimation;

	Scene m_SceneUI;
};