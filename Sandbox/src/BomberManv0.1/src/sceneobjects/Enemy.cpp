#include "Enemy.h"

#include "../scripts/EnemyMovementController.h"

void EnemySpawner::Init(Scene* scene, GameGrid* gamegrid, Player* player)
{
	// Initializing global variables
	g_GameScene = scene;
	g_Player = player;
	g_GameGrid = gamegrid;
}

void EnemySpawner::Spawn(const MonsterStats& stats)
{
	Monster monster;
	monster.MonsterEntity = g_GameScene->CreateEntityWithDrawOrder(3, "EnemyEntity");
	monster.MonsterEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY;
	monster.MonsterEntity.AddCustomComponent<MonsterStats>(stats);
	monster.MonsterEntity.AddComponent<SpriteRendererComponent>();
	// Setings up animations
	auto& fa = monster.MonsterEntity.AddCustomComponent<FrameAnimator>(monster.MonsterEntity);

	// Setting up physics
	g_GameGrid->ForEach_1([&](Entity gridentity)
	{
		if (gridentity.GetComponent<EntityTypeComponent>().Type == EntityType::TILE_MSP)
		{
			monster.MonsterEntity.GetComponent<TransformComponent>().Translation 
				= gridentity.GetComponent<TransformComponent>().Translation;
			return true;
		}
		return false;
	});

	float scale = 0.96f;
	auto& scale_cmp = monster.MonsterEntity.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

	auto& rb2d = monster.MonsterEntity.AddComponent<Rigidbody2DComponent>();
	rb2d.Gravity = false;
	rb2d.FixedRotation = true;
	rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;

	auto& cc2d = monster.MonsterEntity.AddComponent<CircleCollider2DComponent>();
	cc2d.Radius = scale / 2.0f;
	cc2d.Friction = 0.0f;
	//cc.Trigger = true;

	// Adding movement script
	monster.MonsterEntity.AddComponent<NativeScriptComponent>().Bind<EnemyMovementController>();
}

void EnemySpawner::OnUpdate(Timestep ts)
{
	for (auto& monsters : m_MonsterList)
	{
		auto& fa = monsters.MonsterEntity.GetComponent<FrameAnimator>();
		if (fa.IsAnyPlaying())
			fa.OnUpdate(ts);
	}

}
