#pragma once
#include <Hurikan.h>

using namespace Hurikan;

class Entity
{
	virtual void Draw() = 0;
	virtual void Update(Timestep& ts) = 0;
};
