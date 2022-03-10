#pragma once

#include <vector>
#include <string>

#define MIN_LEVEL_WIDTH 32
#define MIN_LEVEL_HEIGHT 15

#define MAX_RANDOM_LEVEL_WIDTH 40
#define MAX_RANDOM_LEVEL_HEIGHT 23

struct Level
{
	uint16_t ID;

	std::string MapSkeleton = std::string();
	int Width = 0, Height = 0;
	int Time = 0;
	bool BossLevel = false;

	inline bool OverScreen() const
	{
		return Width > MIN_LEVEL_WIDTH || Height > MIN_LEVEL_HEIGHT;
	}
};

class LevelLoader
{
public:
	static void Deserialize(std::vector<Level>& levels);
	static void EraseLevelsFile();
	static void GetLevelWithGeneratedMap(Level& level);
private:
	static void SerializeLevels(std::vector<Level>& levels);
	static void GetBossLevel(Level& level);
};

