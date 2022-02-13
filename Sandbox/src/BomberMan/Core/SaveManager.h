#pragma once

#include "BomberMan/Core/Observer.h"

#include <string>
#include <vector>

#define MIN_LEVEL_WIDTH 32.0f
#define MIN_LEVEL_HEIGHT 15.0f

#define MAX_RANDOM_LEVEL_WIDTH 40
#define MAX_RANDOM_LEVEL_HEIGHT 23

struct Level
{
	uint8_t ID;

	std::string MapSkeleton = std::string();
	int Width = 0, Height = 0;
	int CurrentTime = 0;
	int Time = 0;

	inline bool OverScreen()
	{
		return Width > MIN_LEVEL_WIDTH || Height > MIN_LEVEL_HEIGHT;
	}

};

enum class What
{
	LEVEL = 0,
	UPGRADES,
	SCORE,
};

#pragma pack(push, 1)
struct GameData
{
	bool AlreadyPlayed = false;
	uint16_t CurrentLevel = 0;
	uint16_t Upgrades = 0; // "Flags"
	uint16_t Score = 0;
};
#pragma pack(pop)

class SaveManager
{
public:
	static bool Init() { return s_Instance.Init_Impl(); }
	static void Save() { return s_Instance.Save_Impl(); }
	static void ResetProgress() { s_Instance.Reset_Impl(); }
	static void ResetCurrentLevel() { s_Instance.ResetCurrentLevel_Impl(); }
	static void NextLevel() { s_Instance.NextLevel_Impl(); }

	static GameData& Data() { return s_Instance.m_GameData; }
	static Level& GetCurrentLevel() { return s_Instance.m_CurrentLevel; }
	
	static inline void UpdateClock() { s_Instance.m_CurrentLevel.CurrentTime--; };

	static inline void AddScore(int score) { s_Instance.m_GameData.Score += score; }

	static inline bool AlreadyPlayed() { return s_Instance.m_GameData.AlreadyPlayed; }
private:
	SaveManager() = default;

	bool Init_Impl();
	void Reset_Impl();
	void NextLevel_Impl();

	void Save_Impl(); 
	void ResetCurrentLevel_Impl();
	void LoadLevels();
	Level GenerateRandomMap(const Level& level);
private:
	bool m_NewGame = false;

	std::vector<Level> m_Levels;

	GameData m_GameData;
	Level m_CurrentLevel;

	static SaveManager s_Instance;
};