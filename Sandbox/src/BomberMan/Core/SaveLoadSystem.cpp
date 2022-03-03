#include "SaveLoadSystem.hpp"

#include <Hurikan/Core/Log.h>

#include <fstream>
#include <filesystem>

SaveLoadSystem SaveLoadSystem::s_Instance;

void SaveLoadSystem::Init_Impl()
{
	std::filesystem::create_directory("data");

	LoadData_Impl();
	LoadUserSettings_Impl();

	LevelLoader::Deserialize(m_Levels);
}

void SaveLoadSystem::LoadData_Impl()
{
	std::fstream stream(m_FilePath + "progress.data", std::ios_base::in | std::ios_base::binary);

	if (stream.fail())
	{
		m_FirstTimePlayed = true;
		stream.close();
		CreateDataAndFile_Impl();
		return;
	}
	stream.close();

	LoadDataFromFile_Impl();
}

void SaveLoadSystem::CreateDataAndFile_Impl()
{
	HU_INFO("Created a new file!");

	m_GameData.CompletedLevels = 0;
	m_GameData.SpeedUpgrade = 0;
	m_GameData.BombCountUpgrade = 0;
	m_GameData.BombPowerUpgrade = 0;
	m_GameData.Score = 0;

	std::fstream stream(m_FilePath + "progress.data", std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
	stream.write((const char*)(&m_GameData), sizeof(GameData));
	stream.close();
}

void SaveLoadSystem::LoadDataFromFile_Impl()
{
	HU_INFO("Loaded data from file!");

	std::fstream stream(m_FilePath + "progress.data", std::ios_base::in | std::ios_base::binary);
	stream.read((char*)&m_GameData, sizeof(GameData));
	stream.close();
}

void SaveLoadSystem::SaveData_Impl()
{
	HU_INFO("Saved data to file!");

	std::fstream stream(m_FilePath + "progress.data", std::ios_base::trunc | std::ios_base::out | std::ios_base::binary);
	stream.write((const char*)(&m_GameData), sizeof(GameData));
	stream.close();
}


void SaveLoadSystem::EraseDataAndDeserialize_Impl()
{
	HU_INFO("Erase data!");
	CreateDataAndFile_Impl();

	HU_INFO("Reseting levels!");

	m_Levels.clear();

	LevelLoader::EraseLevelsFile();
	LevelLoader::Deserialize(m_Levels);
}

void SaveLoadSystem::SaveUserSettings_Impl(float music, float sfx)
{
	m_UserSettings.MusicVolume = music;
	m_UserSettings.SfxVolume = sfx;

	// check if file exist
	std::fstream stream(m_FilePath + "usersettings.data", std::ios_base::out | std::ios_base::trunc);

	stream << "Music=" << std::to_string((int)(m_UserSettings.MusicVolume * 100)) << "\n";
	stream << "Sfx=" << std::to_string((int)(m_UserSettings.SfxVolume * 100));

	stream.close();
}

void SaveLoadSystem::LoadUserSettings_Impl()
{
	// check if file exist
	std::fstream stream(m_FilePath + "usersettings.data", std::ios_base::in | std::ios_base::out);

	// If file not exist
	if (stream.fail())
	{
		stream.close();

		stream.open(m_FilePath + "usersettings.data", std::ios_base::out | std::ios_base::trunc);

		stream << "Music=100\n";
		stream << "Sfx=100";

		stream.close();

		return;
	}

	std::string music, sfx;
	std::getline(stream, music);
	std::getline(stream, sfx);

	m_UserSettings.MusicVolume = std::stoi(music.substr(6, 6)) / 100.0f;
	m_UserSettings.SfxVolume = std::stoi(sfx.substr(4, 4)) / 100.0f;
}

void SaveLoadSystem::Save_Impl(const GameData& data)
{
	m_GameData = data;
	SaveData_Impl();
}

