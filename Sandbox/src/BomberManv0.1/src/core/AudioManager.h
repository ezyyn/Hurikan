#pragma once
#include <Hurikan.h>

#include <HazelAudio/src/HazelAudio.h>

enum class AudioType
{
	LEVEL_START = 0,
	LEVEL_THEME,
	LEVEL_ALL_MONTERS_KILLED,
	LEVEL_FIND_THE_DOOR,
	LEVEL_FAILED,
	LEVEL_COMPLETED,

	BOMB_PLACED,  
	BOMB_EXPLODED,

	PLAYER_HORIZONTAL_MOVEMENT,
	PLAYER_VERTICAL_MOVEMENT,
	PLAYER_DEATH,

	ITEM_PICKUP,
};

struct AudioData
{
	AudioType Type;
	Hazel::AudioSource SourceHandle;
};

class AudioManager
{
public:
	AudioManager(const AudioManager&) = delete;
	~AudioManager();
public:
	static AudioManager& GetInstance() { return s_Instance; }

	static void Init() { GetInstance().InitImpl(); }
	static void Play(AudioType audioType) { GetInstance().PlayImpl(audioType); }
	static void Stop(AudioType audioType) { GetInstance().StopImpl(audioType); }
	static void CheckStates() { GetInstance().CheckImpl(); }
private:
	AudioManager() = default;
	void InitImpl();
	void PlayImpl(AudioType audioType);
	void StopImpl(AudioType audioType);
	void CheckImpl();

	std::vector<AudioData> m_AudioSources;

	static AudioManager s_Instance;
};

