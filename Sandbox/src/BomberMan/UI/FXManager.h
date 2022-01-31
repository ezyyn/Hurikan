#pragma once

#include "BomberMan/UI/Effects.h"
#include "BomberMan/Core/Observer.h"

class FXManager : public Observer
{
public:
	void Init();
	void OnUpdate(Timestep& ts);
private:
	std::vector<Effect> m_EffectsList;
};