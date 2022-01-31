#include "SaveManager.h"

#include "BomberMan/Core/LevelManager.h"

#include <iostream>
#include <fstream>

SaveManager SaveManager::s_Instance;

void SaveManager::Save(What what)
{
	std::fstream levels_file;

	levels_file.open("progress.data", std::ios::in | std::ios::binary);
	levels_file.read((char*)&s_Instance.m_GameData, sizeof(GameData));
	levels_file.close();

	if (what == What::LEVEL)
	{
		s_Instance.m_GameData.CurrentLevel = LevelManager::GetCurrentLevel().ID;
	}
	else if (what == What::SCORE)
	{
		s_Instance.m_GameData.Score = 200;
	}
	else if (what == What::UPGRADES)
	{
		s_Instance.m_GameData.Upgrades = 45;
	}

	levels_file.open("progress.data", std::ios::out | std::ios::binary | std::ios::trunc);
	levels_file.write((const char*)(&s_Instance.m_GameData), sizeof(GameData));
	levels_file.close();
}

void SaveManager::Init()
{
	// Checks the overall progress of the player
	std::fstream levels_file;

	levels_file.open("progress.data", std::ios::in | std::ios::binary);

	// file has not been found so create new ->erasing all player progress
	/*
		attributes needs to be saved:
		 - Level
		 - Upgrades
		 - Score66521297
	*/

	if (levels_file.fail())
	{
		levels_file.close();
		levels_file.open("progress.data", std::ios::out | std::ios::binary);

		GameData buffer;
		buffer.CurrentLevel = 0;
		buffer.Score = 0;
		buffer.Upgrades = 0;
		levels_file.write((const char*)&buffer, sizeof(buffer));
		levels_file.close();


		s_Instance.m_GameData = buffer;
		s_Instance.m_NewGame = true;
	}
}

std::vector<Level> SaveManager::LoadLevels()
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

	return levels;
}
