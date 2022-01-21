#pragma once

#include <string>

struct Level
{
	std::string MapSkeleton = std::string();
	int Width = 0, Height = 0;
	int CurrentTime = 0;
	int Time = 0;
};

class LevelManager
{
public:
	static inline void Init() { s_Instance.Init_Impl(); }

	static inline const Level& GetCurrentLevel() { return s_Instance.m_CurrentLevel; }
private:
	void Init_Impl();
private:
	Level m_CurrentLevel;

	/*
		TODO: LevelMap
	*/

	static LevelManager s_Instance;
};