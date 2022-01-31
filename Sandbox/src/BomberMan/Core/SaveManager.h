#pragma once

#include <string>
#include <vector>

enum class What
{
	LEVEL = 0,
	UPGRADES,
	SCORE,
};

#pragma pack(push, 1)
struct GameData
{
	uint16_t CurrentLevel;
	uint16_t Upgrades; // "Flags"
	uint16_t Score;
};
#pragma pack(pop)

struct Level
{
	uint8_t ID;

	std::string MapSkeleton = std::string();
	int Width = 0, Height = 0;
	int CurrentTime = 0;
	int Time = 0;


	inline bool OverScreen()
	{
		return Width > 32 && Height > 15;
	}
};

class SaveManager
{
public:
	static void Init();

	static GameData& Data() { return s_Instance.m_GameData; }
	static std::vector<Level> LoadLevels();

	void Save(What save);
private:
	SaveManager() = default;

	bool m_NewGame = false;
	GameData m_GameData;

	static SaveManager s_Instance;
};