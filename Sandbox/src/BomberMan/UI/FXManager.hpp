#pragma once

#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/UI/Effects.hpp"

class FXManager : public Observer
{
public:
	~FXManager();

	void Init(Scene* scene);
	void OnUpdate(Timestep& ts);
private:
	virtual void OnGameEvent(GameEvent& e) override;
private:
	std::vector<Effect*> m_PlayingEffects;

	Scene* m_CurrentScene;
};