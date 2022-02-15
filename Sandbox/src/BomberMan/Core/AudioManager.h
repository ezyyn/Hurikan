#pragma once

#include "BomberMan/Core/Observer.h"

#include <string>

class AudioManager
{
public:
	static void Init();
	static void Shutdown();

	static void Play(const std::string& name);
	static void Stop(const std::string& name);
	static void Pause(const std::string& name);
	static void Unpause(const std::string& name);
private:
	AudioManager() = default;
};

class AudioAssistant : public Observer
{
public:
	/*AudioAssistant();
	~AudioAssistant();*/

	// Inherited via Observer
	virtual void OnGameEvent(GameEvent& e) override;

private:
};