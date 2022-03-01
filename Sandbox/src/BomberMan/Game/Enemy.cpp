#include "Enemy.hpp"

#include "BomberMan/Core/Navigation.hpp"

#include "BomberMan/Game/EnemyClasses/RegularEnemy.hpp"
#include "BomberMan/Game/EnemyClasses/FastEnemy.hpp"
#include "BomberMan/Game/EnemyClasses/SmartEnemy.hpp"

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
		/*auto enemy = new SmartEnemy(g_GameScene, grid_entity);
		m_Enemies.push_back(enemy);
		enemy->Attach(this);*/
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
