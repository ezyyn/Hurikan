#include "SaveManager.h"
#include "BomberMan/Core/Utils.h"

SaveManager SaveManager::s_Instance;

#include <fstream>
#include <algorithm>

bool SaveManager::Init_Impl()
{
	LoadLevels();

	// Checks the overall progress of the player
	std::fstream progress;

	progress.open("progress.data", std::ios::in | std::ios::binary);

	// file has not been found so create new ->erasing all player progress
	/*
		attributes needs to be saved:
		 - Level
		 - Upgrades
		 - Score66521297
	*/

	if (progress.fail())
	{
		progress.close();

		/*levels_file.open("progress.data", std::ios::out | std::ios::binary);

		GameData buffer;
		buffer.CurrentLevel = 0;
		buffer.Score = 0;
		buffer.Upgrades = 0;
		levels_file.write((const char*)&buffer, sizeof(buffer));
		levels_file.close();

		m_GameData = buffer;*/
		m_CurrentLevel = m_Levels[0];
		Save();
		m_NewGame = true;
		return true;
	}

	// Successfully found the file and now reading from it
	//progress.open("progress.data", std::ios::in | std::ios::binary);

	if (progress.bad())
	{
		__debugbreak();
	}
	
	progress.read((char*)&m_GameData, sizeof(GameData));

	m_CurrentLevel = m_Levels[m_GameData.CurrentLevel];

	progress.close();
	return false;
}

void SaveManager::Reset_Impl()
{
	m_CurrentLevel = m_Levels[0];
	m_GameData.CurrentLevel = 0;
	m_GameData.Upgrades = 0;
	m_GameData.Score = 0;
	Save_Impl();
}

void SaveManager::NextLevel_Impl()
{
	// Save current progress
	if(++m_GameData.CurrentLevel < m_Levels.size() )
		m_CurrentLevel = m_Levels[m_GameData.CurrentLevel];
	else
	{
		__debugbreak();
	}
	Save_Impl();
}

void SaveManager::Save_Impl()
{
	std::fstream progress;

	/*levels_file.open("progress.data", std::ios::in | std::ios::binary);
	levels_file.read((char*)&m_GameData, sizeof(GameData));
	levels_file.close();*/

	//m_GameData.CurrentLevel = LevelManager::GetCurrentLevel().ID;
	//m_GameData.Score = 200;
	//m_GameData.Upgrades = 45;

	progress.open("progress.data", std::ios::out | std::ios::binary | std::ios::trunc);
	progress.write((const char*)(&m_GameData), sizeof(GameData));
	progress.close();
}

void SaveManager::ResetCurrentLevel_Impl()
{
	m_CurrentLevel.CurrentTime = m_CurrentLevel.Time;
}

void SaveManager::LoadLevels()
{
	std::vector<Level> levels;

	Level firstLevel;

	std::fstream levels_file;

	levels_file.open("levels.data", std::ios::in);

	if (levels_file.fail())
	{
		levels_file.close();
		__debugbreak(); // File do not exist what now lol
	}

	// Reading a file
	std::string line;
	while (std::getline(levels_file, line))
	{
		if (line.compare("{") == 0)
		{
			// New level
			Level read_level;

			// Level line
			std::getline(levels_file, line);

			if (line.find("Level=") != std::string::npos)
			{
				read_level.ID = std::stoi(line.substr(6, line.size() - 1));
			}
			// Time line
			std::getline(levels_file, line);

			if (line.find("Time=") != std::string::npos)
			{
				read_level.Time = read_level.CurrentTime = std::stoi(line.substr(5, line.size() - 1));
			}

			// Map
			while (std::getline(levels_file, line))
			{
				if (line[0] == 'R')
				{
					read_level = GenerateRandomMap(read_level);
					break;
				}

				if (line.compare("}") == 0)
					break;

				read_level.Height++;
				read_level.Width = line.size();
				read_level.MapSkeleton += line;
			}

			levels.push_back(read_level);
		}
	}

	levels_file.close();

	m_Levels = levels;
}

Level SaveManager::GenerateRandomMap(const Level& level)
{
	Level random_level;

	random_level.ID = level.ID;
	random_level.Time = level.Time;
	random_level.CurrentTime = level.CurrentTime;
	/*random_level.Width  = (rand() % MAX_RANDOM_LEVEL_WIDTH) + MIN_LEVEL_WIDTH;
	random_level.Height = (rand() % MAX_RANDOM_LEVEL_HEIGHT) + MIN_LEVEL_HEIGHT;*/

	random_level.Width = 33;
	random_level.Height = 15;

	std::string map;

	struct Position
	{
		Position(int x,int y) : X(x), Y(y) {}

		bool operator==(const Position& other)
		{
			return X == other.X && Y == other.Y;
		}

		int X,
			Y;
	};

	std::vector<Position> emptyTiles;

	for (int y = 0; y < random_level.Height; ++y)
	{
		std::string line;

		for (int x = 0; x < random_level.Width; ++x)
		{
			char c = '-';
			if (x == 0 || x == random_level.Width - 1)
			{
				c = '#';
			} else if (x % 2 == 0 && y % 2 == 0)
			{
				c = '#';
			} else if (y == 0 || y == random_level.Height - 1)
			{
				c = '#';
			}

			if (c == '-')
			{
				emptyTiles.emplace_back(x,y);
			}

			line.append(1, c);
		}
		map += line;
	}

	Position player_pos =	emptyTiles[Utils::Random(0, emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), player_pos), emptyTiles.end());
	Position key_pos =		emptyTiles[Utils::Random(0, emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), key_pos), emptyTiles.end());
	Position exit_pos =		emptyTiles[Utils::Random(0, emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), exit_pos), emptyTiles.end());


	for (int y = 0; y < random_level.Height; ++y)
	{
		int random_number_enemies_per_line = Utils::Random(0, 3);
		for (int x = 0; x < random_level.Width; ++x)
		{
			if (x == player_pos.X && y == player_pos.Y)
			{
				map[y * random_level.Width + x] = 'P';
				continue;
			} else
			if (x == key_pos.X && y == key_pos.Y)
			{
				map[y * random_level.Width + x] = 'K';
				continue;
			} else
			if (x == exit_pos.X && y == exit_pos.Y)
			{
				map[y * random_level.Width + x] = 'U';
				continue;
			}

			if (map[y * random_level.Width + x] == '-')
			{
				if (Utils::Random(0, 5) > 3)
				{
					map[y * random_level.Width + x] = 'B';
				}
				if (random_number_enemies_per_line > 0)
				{
					if (Utils::Random(0, 10) > 3)
					{
						map[y * random_level.Width + x] = 'M';
						random_number_enemies_per_line--;
					} else 
					{
						map[y * random_level.Width + x] = 'R';
						random_number_enemies_per_line--;
					}
				}
			}
		}
	}

	auto& replace = [](char& pos)
	{
		if (pos == 'B' || pos == 'M')
			pos = '-';
	};

	for (int y = 0; y < random_level.Height; ++y)
	{
		for (int x = 0; x < random_level.Width; ++x)
		{
			if (map[y * random_level.Width + x] == 'P')
			{
				// top down
				replace(map[y * random_level.Width + x + 1]);
				replace(map[y * random_level.Width + x - 1]);
				replace(map[(y - 1) * random_level.Width + x]);
				replace(map[(y + 1) * random_level.Width + x]);

				// diagonal
				replace(map[(y+1) * random_level.Width + x+1]);
				replace(map[(y-1) * random_level.Width + x-1]);
				replace(map[(y-1) * random_level.Width + x+1]);
				replace(map[(y+1) * random_level.Width + x-1]);
			}
		}
	}

	random_level.MapSkeleton = map;

	return random_level;
}
