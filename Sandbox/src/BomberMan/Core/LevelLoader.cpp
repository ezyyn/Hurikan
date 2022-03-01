#include "LevelLoader.hpp"

#include "Utils.hpp"

#include <fstream>
#include <filesystem>

struct Position
{
	Position(int x, int y) : X(x), Y(y) {}

	bool operator==(const Position& other)
	{
		return X == other.X && Y == other.Y;
	}

	int X,
		Y;
};

void LevelLoader::Deserialize(std::vector<Level>& levels)
{
	bool default_loaded = false;

	std::fstream levels_file;

	levels_file.open("levels.data", std::ios::in);

	if (levels_file.fail())
	{
		levels_file.close();
		levels_file.open("default_levels.data", std::ios::in);
		default_loaded = true;
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
				read_level.Time = std::stoi(line.substr(5, line.size() - 1));
			}

			// Map
			while (std::getline(levels_file, line))
			{
				/*if (default_loaded && line[0] == 'R') // TODO: Noise algorithm
				{
					GetLevelWithGeneratedMap(read_level);
					std::getline(levels_file, line);
					break;
				}
				else if (default_loaded && line[0] == 'S')
				{
					std::getline(levels_file, line);
					break;
				}*/
				if (line.compare("}") == 0)
					break;

				read_level.Height++;
				read_level.Width = (int)line.size();

				if(line.find("S") != std::string::npos)
				{
					read_level.BossLevel = true;
				}
				read_level.MapSkeleton += line;
			}

			levels.push_back(read_level);
		}
	}
	levels_file.close();

	if (default_loaded)
	{
		SerializeLevels(levels);
	}
}

void LevelLoader::GetLevelWithGeneratedMap(Level& level)
{
	level.Width  = Utils::Random(MIN_LEVEL_WIDTH, MAX_RANDOM_LEVEL_WIDTH);
	level.Height = Utils::Random(MIN_LEVEL_HEIGHT, MAX_RANDOM_LEVEL_HEIGHT);

	level.Width = 33;
	level.Height = 15;

	std::string map;

	std::vector<Position> emptyTiles;

	for (int y = 0; y < level.Height; ++y)
	{
		std::string line;

		for (int x = 0; x < level.Width; ++x)
		{
			char c = '-';
			if (x == 0 || x == level.Width - 1)
			{
				c = '#';
			}
			else if (x % 2 == 0 && y % 2 == 0)
			{
				c = '#';
			}
			else if (y == 0 || y == level.Height - 1)
			{
				c = '#';
			}

			if (c == '-')
			{
				emptyTiles.emplace_back(x, y);
			}

			line.append(1, c);
		}
		map += line;
	}

	Position player_pos = emptyTiles[Utils::Random(0, (int)emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), player_pos), emptyTiles.end());
	Position key_pos = emptyTiles[Utils::Random(0, (int)emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), key_pos), emptyTiles.end());
	Position exit_pos = emptyTiles[Utils::Random(0, (int)emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), exit_pos), emptyTiles.end());

	Position bomb_upgrade = emptyTiles[Utils::Random(0, (int)emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), bomb_upgrade), emptyTiles.end());

	Position speed_upgrade = emptyTiles[Utils::Random(0, (int)emptyTiles.size() - 1)];
	emptyTiles.erase(std::remove(emptyTiles.begin(), emptyTiles.end(), speed_upgrade), emptyTiles.end());

	for (int y = 0; y < level.Height; ++y)
	{
		int random_number_enemies_per_line = Utils::Random(0, 3);
		for (int x = 0; x < level.Width; ++x)
		{
			if (x == player_pos.X && y == player_pos.Y)
			{
				map[y * level.Width + x] = 'P';
				continue;
			}
			else
				if (x == key_pos.X && y == key_pos.Y)
				{
					map[y * level.Width + x] = 'K';
					continue;
				}
				else
					if (x == exit_pos.X && y == exit_pos.Y)
					{
						map[y * level.Width + x] = 'U';
						continue;
					}
					else
						if (x == bomb_upgrade.X && y == bomb_upgrade.Y)
						{
							map[y * level.Width + x] = 'W';
							continue;
						}
						else if (x == speed_upgrade.X && y == speed_upgrade.Y)
						{
							map[y * level.Width + x] = 'D';
							continue;
						}

			if (map[y * level.Width + x] == '-')
			{
				if (Utils::Random(0, 5) > 3)
				{
					map[y * level.Width + x] = 'B';
				}
				if (random_number_enemies_per_line > 0)
				{
					if (Utils::Random(0, 10) > 3)
					{
						map[y * level.Width + x] = 'M';
						random_number_enemies_per_line--;
					}
					else
						if (Utils::Random(0, 10) > 7)
						{
							map[y * level.Width + x] = 'A';

							if (map[y * level.Width + x] != 'M')
								random_number_enemies_per_line--;
						}
				}
			}
		}
	}

	auto replace = [](char& pos)
	{
		if (pos == 'B' || pos == 'M')
			pos = '-';
	};

	for (int y = 0; y < level.Height; ++y)
	{
		for (int x = 0; x < level.Width; ++x)
		{
			if (map[y * level.Width + x] == 'P')
			{
				// top down
				replace(map[y * level.Width + x + 1]);
				replace(map[y * level.Width + x - 1]);
				replace(map[(y - 1) * level.Width + x]);
				replace(map[(y + 1) * level.Width + x]);

				// diagonal
				replace(map[(y + 1) * level.Width + x + 1]);
				replace(map[(y - 1) * level.Width + x - 1]);
				replace(map[(y - 1) * level.Width + x + 1]);
				replace(map[(y + 1) * level.Width + x - 1]);
			}
		}
	}

	{
		// TODO: REMOVE
		bool
			keycheck = false,
			exitcheck = false,
			playerposcheck = false;

		for (int i = 0; i < map.size(); ++i)
		{
			if (map[i] == 'K') {
				keycheck = true;
			}
			else
				if (map[i] == 'U') {
					exitcheck = true;
				}
				else
					if (map[i] == 'P') {
						playerposcheck = true;
					}
		}

		//HU_CORE_ASSERT(keycheck && exitcheck && playerposcheck, "Some of those vars are not in map!");
	}

	level.MapSkeleton = map;
}

void LevelLoader::SerializeLevels(std::vector<Level>& levels)
{
	std::fstream stream("levels.data", std::ios_base::out | std::ios_base::trunc);

	for (auto& level : levels)
	{
		stream << "{\n";
		stream << "Level=" << level.ID << "\n";
		stream << "Time=" << level.Time << "\n";

		for (int i = 0; i < level.Height; ++i)
		{
			for (int j = 0; j < level.Width; ++j)
			{
				stream << level.MapSkeleton[i * level.Width + j];
			}
			stream << "\n";
		}
		stream << "}\n";
	}

	stream.close();
}

void LevelLoader::EraseLevelsFile()
{
	std::remove("levels.data");
}

void LevelLoader::GetBossLevel(Level& level)
{
	level.Width = 36;
	level.Height = 18;

	std::vector<Position> emptyTiles;

	for (int y = 0; y < level.Height; ++y)
	{
		std::string line;

		for (int x = 0; x < level.Width; ++x)
		{
			char c = '-';
			if (x == 0 || x == level.Width - 1)
			{
				c = '#';
			}
			else if (x % 2 == 0 && y % 2 == 0)
			{
				c = '#';
			}
			else if (y == 0 || y == level.Height - 1)
			{
				c = '#';
			}

			if (c == '-')
			{
				emptyTiles.emplace_back(x, y);
			}

			line.append(1, c);
		}
		level.MapSkeleton += line;
	}
	
	int half_w = level.Width / 2;
	int half_h = level.Height / 2;

	level.MapSkeleton[half_w * half_h + half_h] = 'P';
	level.MapSkeleton[half_w * half_h + (half_h / 2)] = 'S'; // Boss position

}


