#pragma once

#include "BomberMan/Core/Observer.hpp"

#include <Hurikan/Core/KeyCodes.h>
#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

enum class SettingsMenuOption
{
	MUSIC = 0,
	SFX,
	SAVE,
	CANCEL,
	NONE
};

class MainMenuSettings : public Observable
{
public:
	void Init(Scene* scene, Entity& arrowHead);

	void Show(bool show);

	inline bool IsDisplayed() const { return m_Displayed; }

	void OnKeyPressed(KeyCode key);
private:
	void UpdateMusic();
	void UpdateSfx();
private:
	Entity
		m_SettingsOptions,
		m_ArrowHead;

	Entity 
		m_MusicPercentage[3],
		m_SfxPercentage[3];

	int 
		m_MusicCounter = 0,
		m_SfxCounter = 0;

	glm::vec4 m_ConfirmedOptionColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);

	SettingsMenuOption
		m_SelectedOption = SettingsMenuOption::MUSIC,
		m_ConfirmedOption = SettingsMenuOption::NONE;

	bool m_Displayed = false;
};