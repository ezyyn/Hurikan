#pragma once

#include "BomberMan/Core/SaveManager.h"

class LevelManager
{
public:
	static inline void Init() { s_Instance.Init_Impl(); }

	static inline Level& GetCurrentLevel() { return s_Instance.m_CurrentLevel; }
private:
	void Init_Impl();
private:
	Level m_CurrentLevel;

	std::vector<Level> m_Levels;
	/*
		TODO: LevelMap
	*/

	static LevelManager s_Instance;
};