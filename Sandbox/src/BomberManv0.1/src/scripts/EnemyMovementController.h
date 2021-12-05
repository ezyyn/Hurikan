#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include <box2d/b2_body.h>
#include "../sceneobjects/Enemy.h"

class EnemyMovementController : public ScriptableEntity
{
protected:
	void OnCreate() override
	{
		m_MonsterBody = (b2Body*)GetComponent<Rigidbody2DComponent>().RuntimeBody;
		m_Stats = &GetComponent<MonsterStats>();
	}

	void OnDestroy() override
	{
	}

	void OnUpdate(Timestep ts) override
	{
		//Direction = { 0, 0 };

		Velocity.x = Direction.x * m_Stats->Speed;
		Velocity.y = Direction.y * m_Stats->Speed;
		
		m_MonsterBody->SetLinearVelocity(Velocity);
	}
public:
	void BeginContact(Entity a, Entity enemy)
	{
		switch(a.GetComponent<EntityTypeComponent>().Type)
		{
			case EntityType::TILE_BOX:
			case EntityType::TILE_WALL:
			{
				// Do some logic
				
				break;
			}
		}
	}
	void EndContact(Entity a, Entity enemy)
	{
		switch (a.GetComponent<EntityTypeComponent>().Type)
		{
		case EntityType::TILE_BOX:
		case EntityType::TILE_WALL:
		{
			// Do some logic

			break;
		}
		}
	}
private:
	b2Vec2 Velocity = { 0.0f,0.0f };
	b2Vec2 Direction = { 0.0f,0.0f };
	
	MonsterStats* m_Stats;
	b2Body* m_MonsterBody;
};