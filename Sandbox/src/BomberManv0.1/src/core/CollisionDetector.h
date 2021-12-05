#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>

#include "../scripts/EnemyMovementController.h"

class CollissionDetector : b2ContactListener
{
public:
	std::pair<Entity, Entity> ExtractEntities(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* colliderA = fixtureA->GetBody();
		b2Body* colliderB = fixtureB->GetBody();

		Entity* ptrA = (Entity*)colliderA->GetUserData().pointer;
		Entity* ptrB = (Entity*)colliderB->GetUserData().pointer;

		return std::make_pair(*ptrA, *ptrB);
	}

	void Init(Scene* scene)
	{
		g_GameScene = scene;
	}

	bool IsColliding = false;

	void BeginContact(b2Contact* contact) override
	{
		auto& [entityA, entityB] = ExtractEntities(contact);

		if (entityA.GetComponent<EntityTypeComponent>().Type == EntityType::BOMB_SPREAD_EXPLOSION
			&& entityB.GetComponent<EntityTypeComponent>().Type == EntityType::PLAYER)
		{
			IsColliding = true;
		}
	}

	void EndContact(b2Contact* contact) override
	{
		auto& [entityA, entityB] = ExtractEntities(contact);

		if (entityA.GetComponent<EntityTypeComponent>().Type == EntityType::BOMB_SPREAD_EXPLOSION
			&& entityB.GetComponent<EntityTypeComponent>().Type == EntityType::PLAYER)
		{
			IsColliding = false;
		}

		if (entityA.GetComponent<EntityTypeComponent>().Type == EntityType::BOMB)
		{
			auto* rb = (b2Body*)entityA.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			rb->GetFixtureList()[0].SetSensor(false);
		}

	}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
	}

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{
	}
private:
	void DamagePlayer(Entity player)
	{
		player.GetComponent<PlayerStats>().Health--; // TODO: move it to the player( possible GameManager )
	}

	Scene* g_GameScene;
};