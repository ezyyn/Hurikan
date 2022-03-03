#include "Enemy.hpp"

#include "BomberMan/Game/EnemyClasses/RegularEnemy.hpp"
#include "BomberMan/Game/EnemyClasses/FastEnemy.hpp"
#include "BomberMan/Game/EnemyClasses/SmartEnemy.hpp"
#include "BomberMan/Game/EnemyClasses/Boss.hpp"

EnemySpawner::~EnemySpawner()
{
	while (!m_ErasePool.empty())
	{
		delete m_ErasePool.front();
		m_ErasePool.pop_front();
	}

	for (auto enemy : m_Enemies)
	{
		delete enemy;
	}
}

////////////////////////////////////////////////////////////////////
/////////////////////// Enemy Spawner //////////////////////////////
////////////////////////////////////////////////////////////////////

void EnemySpawner::Init(Scene* scene)
{
	g_GameScene = scene;
}

void EnemySpawner::Spawn(Entity& grid_entity)
{
	if (grid_entity.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_REGULAR)
	{
		auto& e = g_GameScene->CreateEntityWithDrawOrder(2);

		auto enemy = new RegularEnemy(e, grid_entity);
		m_Enemies.push_back(enemy);
		enemy->Attach(this);
	}
	else if(grid_entity.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_FAST)
	{
		auto& e = g_GameScene->CreateEntityWithDrawOrder(2);

		auto enemy = new FastEnemy(e, grid_entity);
		m_Enemies.push_back(enemy);
		enemy->Attach(this);
	}
	else if (grid_entity.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_SMART)
	{
		auto& e = g_GameScene->CreateEntityWithDrawOrder(2);

		auto enemy = new SmartEnemy(e, grid_entity);
		m_Enemies.push_back(enemy);
		enemy->Attach(this);
	}
	else if (grid_entity.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_BOSS)
	{
		auto enemy = new Boss(g_GameScene, grid_entity);
		m_Enemies.push_back(enemy);
		enemy->Attach(this);
	}
}

void EnemySpawner::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::ENEMY_CREATE_NEW) // From grid
	{
		auto data = std::any_cast<Entity>(e.Data);
		Spawn(data);
	}
	else if (e.Type == GameEventType::ENEMY_DEAD)
	{
		auto& dead_enemy = std::any_cast<Entity>(e.Data);

		auto& it = m_Enemies.begin();

		while (it != m_Enemies.end())
		{
			if ((*it)->SameAs(dead_enemy))
			{
				Dispatch(GameEventType::ENEMY_DEAD, dead_enemy);
				g_GameScene->DestroyEntity(dead_enemy);
				m_ErasePool.push_back(*it);
				it = m_Enemies.erase(it);
			} else 
			{
				++it;
			}
		}
	}
	else if (e.Type == GameEventType::ENEMY_MOVED)
	{
		Dispatch(GameEventType::ENEMY_MOVED, e.Data);
	}
	else 
	{
		for (auto& enemy : m_Enemies)
		{
			enemy->OnGameEvent(e);
		}
	}
}

void EnemySpawner::OnUpdate(Timestep& ts)
{
	while (!m_ErasePool.empty())
	{
		delete m_ErasePool.front();
		m_ErasePool.pop_front();
	}

	for (size_t i = 0; i < m_Enemies.size(); ++i)
	{
		m_Enemies[i]->OnUpdate(ts);
	}
}



////////////////////////////////////////////////////////////////////
////////////////////////// Enemy ///////////////////////////////////
////////////////////////////////////////////////////////////////////

Enemy::Enemy(Entity& handle, Entity& grid_entity) : m_Handle(handle), m_LastPositionOnGrid(grid_entity)
{
	m_Handle.Transform().Translation = m_LastPositionOnGrid.Transform().Translation;

	constexpr float scale = 0.96f;
	auto& scale_cmp = m_Handle.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));

	m_Animator = &m_Handle.AddCustomComponent<Animator>();
	m_Animator->SetTarget(m_Handle);
}

void Enemy::Follow(const std::list<Entity>& path)
{
	if (m_Path.empty())
	{
		m_Path = path;
	}
	else
	{
		m_Path.clear();

		m_Path.push_back(m_LastPositionOnGrid);
		for (auto& p : path)
		{
			m_Path.push_back(p);
		}
	}
}

void Enemy::OnUpdate(Timestep& ts)
{
	OnUpdateInternal(ts);
	
	m_Animator->OnUpdate(ts);
	if (m_Hit)
	{
		OnHitUpdate(ts);
	}
	else if (!m_Hit && m_HitColor.g != 1.0f)
	{
		m_HitColor.g = Utils::Lerp(m_HitColor.g, 1.0f, ts * 2);
		m_HitColor.b = Utils::Lerp(m_HitColor.b, 1.0f, ts * 2);
		m_Animator->SetColor(m_HitColor);
	}

	// Checking enemy's health
	if (!m_Alive)
	{
		if (!m_Animator->IsAnyPlaying())
		{
			Dispatch(GameEventType::ENEMY_DEAD, m_Handle);
		}
		return;
	}

	Dispatch(GameEventType::ENEMY_MOVED, m_Handle);

	if (EnemyLogic(ts))
	{
		// Movement and animation
		auto& transform = m_Handle.Transform();

		m_LastPositionOnGrid = m_Path.front();

		m_PreviousPosition = transform.Translation;

		transform.Translation.x = Utils::Lerp(m_Handle.Transform().Translation.x, m_Path.front().Transform().Translation.x, ts * m_Properties.Speed);
		transform.Translation.y = Utils::Lerp(m_Handle.Transform().Translation.y, m_Path.front().Transform().Translation.y, ts * m_Properties.Speed);

		if (m_PreviousPosition.x < transform.Translation.x && m_CurrentDirection != Direction::RIGHT)
		{
			m_CurrentDirection = Direction::RIGHT;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_IsRotated = false;

			OnChangeDirection(m_CurrentDirection);
		}
		else if (m_PreviousPosition.x > transform.Translation.x && m_CurrentDirection != Direction::LEFT)
		{
			m_CurrentDirection = Direction::LEFT;
			if (!m_IsRotated)
			{
				m_Handle.Transform().Scale.x *= -1;
				m_IsRotated = true;
			}
			OnChangeDirection(m_CurrentDirection);
		}

		if (m_PreviousPosition.y < transform.Translation.y && m_CurrentDirection != Direction::UP)
		{
			m_CurrentDirection = Direction::UP;
			m_IsRotated = false;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			OnChangeDirection(m_CurrentDirection);
		}
		else if (m_PreviousPosition.y > transform.Translation.y && m_CurrentDirection != Direction::DOWN)
		{
			m_CurrentDirection = Direction::DOWN;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_IsRotated = false;
			OnChangeDirection(m_CurrentDirection);
		}

		if (transform.Translation.x == m_Path.front().Transform().Translation.x && transform.Translation.y == m_Path.front().Transform().Translation.y)
		{
			Dispatch(GameEventType::ENEMY_MOVED, m_Handle);
			m_Path.pop_front();
		}
	}
}

void Enemy::OnHitUpdate(Timestep& ts)
{
	m_HitColor.g = Utils::Lerp(m_HitColor.g, 0.0f, ts * 2);
	m_HitColor.b = Utils::Lerp(m_HitColor.b, 0.0f, ts * 2);
	m_Animator->SetColor(m_HitColor);

	if (m_HitColor.g == 0.0f)
	{
		m_Hit = false;
	}
}
