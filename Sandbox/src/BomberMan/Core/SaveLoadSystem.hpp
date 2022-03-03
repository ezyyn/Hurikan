#pragma once

#include "LevelLoader.hpp"

// Data that will be eventually saved/loaded
#pragma pack(push, 1)
struct GameData
{
	uint16_t Score; // Score from previous completed level
	uint8_t SpeedUpgrade; // Upgrades from previous level
	uint8_t BombPowerUpgrade; // Upgrades from previous level
	uint8_t BombCountUpgrade; // Upgrades from previous level
	uint16_t CompletedLevels; // Levels completed so far
};
#pragma pack(pop)

class SaveLoadSystem
{
	struct UserSettings
	{
		float
			MusicVolume = 1.0f,
			SfxVolume = 1.0f;
	};

	/*
		Items to save:
		- Score
		- Upgrades
		- Levels completed so far ( 0 means first level )
		// different file
		- Usersettings
	*/

public:
	/// <summary>
	/// Deserializes player's progress if exist
	/// </summary>
	static inline void Init() { Get().Init_Impl(); }
	/// <summary>
	/// Loads levels
	/// </summary>
	static inline void LoadLevels() { LevelLoader::Deserialize(Get().m_Levels); }

	static inline const GameData& GetGameData() { return Get().m_GameData; }

	static inline void EraseDataAndDeserialize() { Get().EraseDataAndDeserialize_Impl(); }

	static inline void SaveUserSettings(float music, float sfx) { Get().SaveUserSettings_Impl(music, sfx); }

	static inline const UserSettings& GetUserSettings() { return Get().m_UserSettings; }

	static inline const std::vector<Level>& GetLevels() { return Get().m_Levels; }

	static inline void SaveLevel(const GameData& data) { Get().Save_Impl(data); }

	static inline const Level& GetCurrentLevel() { return Get().m_Levels[Get().m_GameData.CompletedLevels]; }
	static inline const Level GetPreviousLevel()
	{ 
		if(Get().m_GameData.CompletedLevels > 0)
			return Get().m_Levels[Get().m_GameData.CompletedLevels - 1]; 

		__debugbreak();
		return {};
	}

	static inline bool AlreadyPlayed() { return Get().m_FirstTimePlayed == false; }

private:
	SaveLoadSystem() = default;
	
	static SaveLoadSystem& Get() { return s_Instance; }

	void Init_Impl();
	void LoadData_Impl();
	void CreateDataAndFile_Impl();
	void LoadDataFromFile_Impl();
	void EraseDataAndDeserialize_Impl();
	void SaveUserSettings_Impl(float music, float sfx);
	void LoadUserSettings_Impl();
	void Save_Impl(const GameData& data);
	void SaveData_Impl();
private:
	bool m_DataFilePresent = false;
	GameData m_GameData;

	std::vector<Level> m_Levels;

	UserSettings m_UserSettings;

	bool m_FirstTimePlayed = false;

	const std::string m_FilePath = "data//";

	static SaveLoadSystem s_Instance;
};
