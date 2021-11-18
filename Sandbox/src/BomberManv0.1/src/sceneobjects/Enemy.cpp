#include "Enemy.h"

#include "../scripts/EnemyMovementController.h"

void Enemy::Init(Scene* scene, Player* player)
{
	m_GameScene = scene;
	m_Player = player;

	m_EnemyEntity = m_GameScene->CreateEntityWithDrawOrder(3, "Enemy");
	m_EnemyEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY;


	// Setting up physics
	auto& rb = m_EnemyEntity.AddComponent<Rigidbody2DComponent>();
	rb.Gravity = false;
	rb.Type = Rigidbody2DComponent::BodyType::Dynamic;
	auto& cc = m_EnemyEntity.AddComponent<CircleCollider2DComponent>();
	cc.Friction = 0.0f;
	cc.Trigger = true;

	m_EnemyEntity.AddComponent<NativeScriptComponent>().Bind<EnemyMovementController>();
}

void Enemy::OnUpdate(Timestep ts)
{
}
