#pragma once

#include "../core/Level.h"
#include "../animations/FrameAnimator.h"

#include <Hurikan.h>
using namespace Hurikan;

class SimpleUI
{
public:
	SimpleUI() = default;
	~SimpleUI() = default;

	void Init(const Level& level);
	void OnUpdate(Timestep ts);
	void Shutdown();
private:
	std::vector<Entity> m_HeartEntities;

	std::vector<Entity> m_TimerCountDown;

	Scene m_SceneUI;
	Entity m_Camera;

	Level m_CurrentLevel;

	Entity m_TopPanel;

	int m_LastState = -1;

	Entity head;

	Ref<SubTexture2D> m_IdleAnimation;
	FrameAnimator m_HeadAnimator;
};