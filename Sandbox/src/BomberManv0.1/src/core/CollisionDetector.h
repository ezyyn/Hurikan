#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>

class CollissionDetector : b2ContactListener
{
public:

	void Init(Scene* scene)
	{
		m_GameScene = scene;
	}

	void BeginContact(b2Contact* contact) override
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* colliderA = fixtureA->GetBody();
		b2Body* colliderB = fixtureB->GetBody();

		Entity* ptrA = (Entity*)colliderA->GetUserData().pointer;
		Entity* ptrB = (Entity*)colliderB->GetUserData().pointer;

		if (ptrA->GetComponent<EntityTypeComponent>().Type == EntityType::TILE_EMPTY)
		{
		//	HU_INFO("{0}, {1}", ptrA->GetComponent<TransformComponent>().Translation.x, ptrA->GetComponent<TransformComponent>().Translation.y);
		}
	}


	void EndContact(b2Contact* contact) override
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* colliderA = fixtureA->GetBody();
		b2Body* colliderB = fixtureB->GetBody();

		Entity* ptrA = (Entity*)colliderA->GetUserData().pointer;
		Entity* ptrB = (Entity*)colliderB->GetUserData().pointer;

		if (ptrA->GetComponent<EntityTypeComponent>().Type == EntityType::BOMB)
		{
			//HU_INFO("dd");
			fixtureA->SetSensor(false);
		}
		//HU_INFO(ptrA->GetComponent<TagComponent>().Tag);
	}


	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
	{
		return;
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* colliderA = fixtureA->GetBody();
		b2Body* colliderB = fixtureB->GetBody();

		Entity* ptrA = (Entity*)colliderA->GetUserData().pointer;
		Entity* ptrB = (Entity*)colliderB->GetUserData().pointer;
	}


	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
	{
	}
private:
	Scene* m_GameScene;
};