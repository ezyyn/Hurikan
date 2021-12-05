#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../sceneobjects/Player.h"

#include <box2d/b2_body.h>

class PlayerScript : public ScriptableEntity
{
protected:
	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(Timestep ts) override;
private:
	b2Vec2 Velocity = { 0.0f,0.0f };
	b2Vec2 Direction = { 0.0f,0.0f };

	bool m_IsRotated = false;
	// References
private:
	PlayerStats* m_Stats = nullptr;
	TransformComponent* m_Transform = nullptr;
	b2Body* m_PlayerBody = nullptr;
	FrameAnimator* m_FrameAnimator = nullptr;

	friend class Player;
};