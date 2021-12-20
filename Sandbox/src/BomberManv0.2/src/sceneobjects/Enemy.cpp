#include "Enemy.h"

#include "../sceneobjects/Grid.h"
#include "../sceneobjects/Player.h"

// Global Variables, maybe make them extern in #Game.h TODO:?
extern Scene* g_GameScene;
extern Player* g_Player;

////////////////////////////////////////////////////////////////////
/////////////////////// Enemy Spawner //////////////////////////////
////////////////////////////////////////////////////////////////////
void EnemySpawner::Init()
{
}

bool EnemySpawner::Spawn(const MonsterProperties& props)
{
	m_MonsterList.push_back({ props });
	return true;
}

void EnemySpawner::OnUpdate(Timestep ts)
{
	for (auto& monster : m_MonsterList)
		monster.OnUpdate(ts);
}

////////////////////////////////////////////////////////////////////
////////////////////////// Enemy ///////////////////////////////////
////////////////////////////////////////////////////////////////////
Monster::Monster(const MonsterProperties& props) : m_Properties(props)
{
	OnSpawn();
}

void Monster::OnSpawn()
{
	m_Handle = g_GameScene->CreateEntityWithDrawOrder(5, "MonsterEntity");

	//m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY;
	//m_Handle.AddCustomComponent<MonsterStats>(stats);
	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
	m_Handle.GetComponent<TransformComponent>().Translation = m_Properties.StartPosition;
	// Setings up animations
	//auto& fa = m_Handle.AddCustomComponent<FrameAnimator>(m_Handle);

	//float scale = 0.96f;
	//auto& scale_cmp = m_Handle.GetComponent<TransformComponent>().Scale;
	//scale_cmp.x = scale;
	//scale_cmp.y = scale;

	auto& rb2d = m_Handle.AddComponent<Rigidbody2DComponent>();
	rb2d.Gravity = false;
	rb2d.FixedRotation = true;
	rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	
	auto& cc2d = m_Handle.AddComponent<CircleCollider2DComponent>();
	//cc2d.Radius = scale / 2.0f;
	cc2d.Friction = 0.0f;
	cc2d.IsSensor = true;
}

void Monster::OnUpdate(Timestep ts)
{
	m_Direction = glm::normalize(g_Player->Position2() - m_Handle.Transform().Translation);

	m_Velocity.x = m_Direction.x * m_Properties.Speed;
	m_Velocity.y = m_Direction.y * m_Properties.Speed;

	m_Body = (b2Body*)m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody;
	m_Body->SetLinearVelocity({ m_Velocity.x, m_Velocity.y });
}

void Monster::DestroyItself()
{
	g_GameScene->DestroyEntity(m_Handle);
}
