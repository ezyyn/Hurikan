#include "AudioManager.hpp"

#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/LevelLoader.hpp"

#include <HurikanAudio/Core/BasicAudioEngine.h>

#include <iostream>

static std::unordered_map<std::string, unsigned int> s_SoundLibrary;

void AudioManager::Init()
{
	Hurikan::AudioListener listener;

	Hurikan::BasicAudio::Init(&listener);

	// Sfx
	Hurikan::SoundInfo sfx;
	sfx.Gain = SaveLoadSystem::GetUserSettings().SfxVolume;
	s_SoundLibrary["BombPlaced"] = Hurikan::BasicAudio::Create("assets/audio/bomb_placed.ogg", sfx);
	s_SoundLibrary["BombExplosion"] = Hurikan::BasicAudio::Create("assets/audio/bomb_explosion.ogg", sfx);

	// Music
	Hurikan::SoundInfo loop;
	loop.Loop = true;
	loop.Gain = SaveLoadSystem::GetUserSettings().MusicVolume;
	s_SoundLibrary["MainMenuLoop"] = Hurikan::BasicAudio::Create("assets/audio/main_menu.ogg", loop);
	s_SoundLibrary["InGameLoop"] = Hurikan::BasicAudio::Create("assets/audio/level_theme_cut.ogg", loop);

	Hurikan::SoundInfo music;
	music.Gain = SaveLoadSystem::GetUserSettings().MusicVolume;
	s_SoundLibrary["LevelStart"] = Hurikan::BasicAudio::Create("assets/audio/level_start.ogg", music);
	s_SoundLibrary["LevelLost"] = Hurikan::BasicAudio::Create("assets/audio/level_lost.ogg", music);
	s_SoundLibrary["LevelCompleted"] = Hurikan::BasicAudio::Create("assets/audio/level_completed.ogg", music);


	Play("MainMenuLoop");
}

void AudioManager::Shutdown()
{
	Hurikan::BasicAudio::Close();
}

void AudioManager::Play(const std::string& name)
{
	Hurikan::BasicAudio::Play(s_SoundLibrary[name]);
}
void AudioManager::Stop(const std::string& name)
{
	Hurikan::BasicAudio::Stop(s_SoundLibrary[name]);
}

void AudioManager::Pause(const std::string& name)
{
	Hurikan::BasicAudio::Pause(s_SoundLibrary[name]);
}

void AudioManager::Unpause(const std::string& name)
{
	Hurikan::BasicAudio::Play(s_SoundLibrary[name]);
}

void AudioManager::UpdateVolume(float music, float sfx)
{
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["BombPlaced"], sfx);
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["BombExplosion"], sfx);
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["MainMenuLoop"], music);
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["InGameLoop"], music);
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["LevelStart"], music);
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["LevelLost"], music);
	Hurikan::BasicAudio::SetGain(s_SoundLibrary["LevelCompleted"], music);
}

// AudioAssistant
// AudioAssistant
// AudioAssistant

void AudioAssistant::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::BOMB_PLACED)
	{
		AudioManager::Play("BombPlaced");
	}
	else if (e.Type == GameEventType::BOMB_EXPLODED)
	{
		AudioManager::Play("BombExplosion");
	} 
	else if (e.Type == GameEventType::AUDIO_LEVEL_SCREEN)
	{
		AudioManager::Stop("MainMenuLoop");
		AudioManager::Play("LevelStart");
	}
	else if (e.Type == GameEventType::RETURN_TO_MAIN_MENU)
	{
		AudioManager::Stop("InGameLoop");
		AudioManager::Play("MainMenuLoop");
	}
	else if (e.Type == GameEventType::AUDIO_PAUSE_INGAME_LOOP)
	{
		AudioManager::Pause("InGameLoop");
	}
	else if (e.Type == GameEventType::AUDIO_UNPAUSE_INGAME_LOOP)
	{
		AudioManager::Unpause("InGameLoop");
	}
	else if (e.Type == GameEventType::AUDIO_LEVEL_FAILED)
	{
		AudioManager::Stop("InGameLoop");
		AudioManager::Play("BombExplosion");
		AudioManager::Play("LevelLost");
	}
	else if (e.Type == GameEventType::AUDIO_LEVEL_SUCCESS)
	{
		AudioManager::Stop("InGameLoop");
		AudioManager::Play("LevelCompleted");
	}
	else if (e.Type == GameEventType::AUDIO_INGAME_LOOP)
	{
		AudioManager::Play("InGameLoop");
	}
	else if (e.Type == GameEventType::CUTSCENE_COMPLETED)
	{
		AudioManager::Play("BossLoop");
	}
}
