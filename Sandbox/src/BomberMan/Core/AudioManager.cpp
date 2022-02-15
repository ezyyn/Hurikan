#include "AudioManager.h"

#include <hua/HurikanAudio.h>

static std::unordered_map<std::string, unsigned int> s_SoundLibrary;

void AudioManager::Init()
{
	Hurikan::AudioListener listener;

	Hurikan::BasicAudio::Init(&listener);
	s_SoundLibrary["BombPlaced"] = Hurikan::BasicAudio::Create("assets/audio/bomb_placed.ogg");
	s_SoundLibrary["BombExplosion"] = Hurikan::BasicAudio::Create("assets/audio/bomb_explosion.ogg");


	Hurikan::SoundInfo loop;
	loop.looping = true;
	s_SoundLibrary["MainMenuLoop"] = Hurikan::BasicAudio::Create("assets/audio/main_menu.ogg", loop);
	s_SoundLibrary["LevelStart"] = Hurikan::BasicAudio::Create("assets/audio/level_start.ogg");
	s_SoundLibrary["InGameLoop"] = Hurikan::BasicAudio::Create("assets/audio/level_theme_cut.ogg", loop);
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
	else if (e.Type == GameEventType::NEW_GAME_CONFIRMED)
	{
		AudioManager::Stop("MainMenuLoop");
		AudioManager::Play("LevelStart");
	}
	else if (e.Type == GameEventType::CONTINUE_CONFIRMED)
	{
		AudioManager::Stop("MainMenuLoop");
		AudioManager::Play("LevelStart");
	}
	else if (e.Type == GameEventType::GAME_LOADED)
	{
		AudioManager::Play("InGameLoop");
	}
	else if (e.Type == GameEventType::RETURN_TO_MAIN_MENU)
	{
		AudioManager::Stop("InGameLoop");
		AudioManager::Play("MainMenuLoop");
	}
	else if (e.Type == GameEventType::GAME_PAUSE)
	{
		AudioManager::Pause("InGameLoop");
	}
	else if (e.Type == GameEventType::GAME_UNPAUSE)
	{
		AudioManager::Unpause("InGameLoop");
	}
	else if (e.Type == GameEventType::GAME_LOST)
	{
		AudioManager::Unpause("InGameLoop");
	}

}
