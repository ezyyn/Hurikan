#pragma once

#include "BomberMan/Core/Animator.hpp"
#include "BomberMan/Core/Observer.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Core/Timestep.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

#include <array>

class SimpleUI : public Observer, public Observable
{
public:
	enum class PauseMenuOption
	{
		NONE,
		MAIN_MENU,
		EXIT
	};

	SimpleUI() = default;
	~SimpleUI() = default;

	void Init();
	void OnUpdate(Timestep& ts, bool paused);
private:
	void DisplayPauseMenu();
	void HidePauseMenu();
	void OnGameEvent(GameEvent& e) override;
	void OnUpdateScore(Timestep& ts);
	void OnUpdateClock(Timestep& ts);
private:
	int m_ScoreOnScreen = 0;
	bool m_UpdateScore = false;
private:
	std::array<Entity, 3>
		m_HeartEntities,
		m_UpgradeBombPower,
		m_UpgradeSpeed,
		m_TimerCountDown;
	std::array<Entity, 5>
		m_ScoreCounter;
	std::array<Entity, 4>
		m_BombEntities;

	Entity
		m_Camera,
		m_TopPanel,
		m_Head,
		m_PauseMenu,
		m_PauseMenuArrowHead,
		m_Key;

	float 
		m_ClockTimer = 1.0f,
		m_ScoreTimer = 0.0f;

	bool m_GameOver = false;
	bool m_PlayerHit = false;
	glm::vec4 m_HitPulseColor = glm::vec4(1.0f);
	int m_AvailableBombs = 1;

	int m_CurrentTime = 0;
	
	int m_PlayerHealth = 3;

	unsigned short m_LastKey = 0;

	PauseMenuOption 
		m_SelectedOption, 
		m_ConfirmedOption;

	Scene m_SceneUI;
};