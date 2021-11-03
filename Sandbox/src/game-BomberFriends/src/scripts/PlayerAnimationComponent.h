#pragma once

#include "FrameAnimationComponent.h"

#include <Hurikan.h>
using namespace Hurikan;

class PlayerAnimationComponent : public FrameAnimationComponent
{
public:
	void OnCreate();
	void OnDestroy();
	void OnUpdate(Timestep ts);
private:
};
