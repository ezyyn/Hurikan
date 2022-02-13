#pragma once

#include "BomberMan/Core/Observer.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
#include <Hurikan/Core/Timestep.h>
#include <Hurikan/Events/KeyEvent.h>

using namespace Hurikan;

enum class MenuOption : int
{
	NEW_GAME = 0,
	CONTINUE,
	SETTINGS,
	SCORE,
	EXIT
};

class MainMenu : public Observable
{
public:
	void Init();

	void OnUpdate(Timestep& ts);

	void UpdateUI();

	void OnKeyPressed(KeyPressedEvent& e);
private:
	Entity 
		m_ArrowHead, 
		m_Camera, 
		m_Logo,
		m_LevelText,
		m_LevelCount;

	Entity 
		m_NewGame,
		m_Continue,
		m_Settings,
		m_Score,
		m_Exit;

	MenuOption 
		m_SelectedOption = MenuOption::NEW_GAME,
		m_ConfirmedOption;


	Scene m_MenuScene;
};