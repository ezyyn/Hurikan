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

	void OnKeyPressed(KeyPressedEvent& e);
private:
	Entity m_ArrowHead, m_Camera;

	MenuOption m_SelectedOption = MenuOption::NEW_GAME,
			   m_ConfirmedOption;

	Scene m_MenuScene;
};