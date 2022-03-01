#pragma once

#include "BomberMan/Core/Observer.hpp"

#include <string>

struct Level;

class AudioManager
{
public:
	static void Init();
	static void Shutdown();

	static void Play(const std::string& name);
	static void Stop(const std::string& name);
	static void Pause(const std::string& name);
	static void Unpause(const std::string& name);

	static void UpdateVolume(float music, float sfx);
private:
	AudioManager() = default;
};

class AudioAssistant : public Observer
{
private:
	virtual void OnGameEvent(GameEvent& e) override;
};