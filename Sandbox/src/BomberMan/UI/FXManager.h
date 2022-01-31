#pragma once

#include "BomberMan/UI/Effects.h"
#include "BomberMan/Core/Observer.h"

class FXManager : public Observer
{
public:
	void Init(Scene* scene);
	void OnUpdate(Timestep& ts);
private:
	// Inherited via Observer
	virtual void OnGameEvent(GameEvent& e) override;

private:
	std::vector<Effect*> m_PlayingEffects;

	Scene* m_CurrentScene;
};