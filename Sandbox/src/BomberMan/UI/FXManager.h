#pragma once

#include "BomberMan/UI/Effects.h"
#include "BomberMan/Core/Observer.h"

class FXManager : public Observer
{
public:
	~FXManager();

	void Init(Scene* scene);
	void OnUpdate(Timestep& ts);
	void CleanUp();
private:
	// Inherited via Observer
	virtual void OnGameEvent(GameEvent& e) override;

private:
	std::vector<Effect*> m_PlayingEffects;

	Scene* m_CurrentScene;
};