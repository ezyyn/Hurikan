#include "AudioManager.h"

#define AL_PLAYING 0x1012
#define AL_PAUSED  0x1013
#define AL_STOPPED 0x1014

AudioManager AudioManager::s_Instance;

AudioManager::~AudioManager()
{
	for (const auto& as : m_AudioSources)
	{
		Hazel::Audio::Delete(as.SourceHandle);
	}
}

void AudioManager::InitImpl()
{
	Hazel::Audio::Init();
}

// TODO: Audio settings to control volume, two types -> SFX volume, Music Volume
void AudioManager::PlayImpl(AudioType audioType)
{
	Hazel::AudioSource* source = nullptr;

	switch (audioType)
	{
	case AudioType::LEVEL_THEME:
	{
		source = &Hazel::AudioSource::LoadFromFile("assets/audio/level_theme.ogg", false);
		source->SetGain(0.5f);
		source->SetLoop(true);
		m_AudioSources.push_back({ audioType, *source });
		break;
	}
	case AudioType::PLAYER_HORIZONTAL_MOVEMENT:
	{
		source = &Hazel::AudioSource::LoadFromFile("assets/audio/player_horizontal_movement.ogg", false);
		source->SetGain(0.5f);
		m_AudioSources.push_back({ audioType, *source});
		break;
	}
	case AudioType::BOMB_PLACED:
	{
		source = &Hazel::AudioSource::LoadFromFile("assets/audio/bomb_placed.ogg", false);
		source->SetGain(0.5f);
		m_AudioSources.push_back({ audioType, *source});
		break;
	}
	case AudioType::PLAYER_DEATH:
	{
		source = &Hazel::AudioSource::LoadFromFile("assets/audio/player_hit_sfx.ogg", false);
		source->SetGain(0.5f);
		m_AudioSources.push_back({ audioType, *source });
		break;
	}
	default:
		__debugbreak();
		break;
	}

	Hazel::Audio::Play(*source);
}

void AudioManager::StopImpl(AudioType audioType)
{
	for (size_t i = 0; i < m_AudioSources.size(); i++)
	{
		switch (audioType)
		{
		case AudioType::LEVEL_THEME:
		{
			Hazel::Audio::Stop(m_AudioSources[i].SourceHandle);
			m_AudioSources.erase(m_AudioSources.begin() + i);
			break;
		}
		default:
			__debugbreak();
			break;
		}
	}
}

void AudioManager::CheckImpl()
{
	for (size_t i = 0; i < m_AudioSources.size(); i++)
	{
		if (Hazel::Audio::CheckState(m_AudioSources[i].SourceHandle) == AL_STOPPED)
		{
			Hazel::Audio::Delete(m_AudioSources[i].SourceHandle);
			m_AudioSources.erase(m_AudioSources.begin() + i);
		}
	}
}

