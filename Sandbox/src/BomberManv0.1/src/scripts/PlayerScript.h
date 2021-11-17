#pragma once

#include <Hurikan.h>
using namespace Hurikan;

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
	float Speed = 5.0f;

	friend class Player;
};