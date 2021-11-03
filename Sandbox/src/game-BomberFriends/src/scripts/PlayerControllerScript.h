#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include <box2d/b2_body.h>

class PlayerController 
{
public:
	PlayerController(Entity entity) : m_PlayerEntity(entity) {}

	void OnUpdate(Timestep ts);
private:
	Entity m_PlayerEntity;

	b2Vec2 Velocity = { 0.0f,0.0f };
	b2Vec2 Direction = { 0.0f,0.0f };
	float Speed = 5.0f;
};