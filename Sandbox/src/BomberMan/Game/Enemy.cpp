#include "Enemy.h"

#include "BomberMan/Core/Navigation.h"

#include "BomberMan/Game/EnemyClasses/RegularEnemy.h"
#include "BomberMan/Game/EnemyClasses/RareEnemy.h"

EnemySpawner::~EnemySpawner()
{
	CleanUp();
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
		auto& e = g_GameScene->CreateEntityWithDrawOrder(1);

		auto enemy = new RegularEnemy(e, grid_entity);
		m_Enemies.push_back(enemy);
		enemy->Attach(this);
	}
	else
	{
		auto& e = g_GameScene->CreateEntityWithDrawOrder(1);

		auto enemy = new RareEnemy(e, grid_entity);
		m_Enemies.push_back(enemy); // todo: polymorphism
		enemy->Attach(this);
	}
}

void EnemySpawner::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::CREATE_NEW_ENEMY) // From grid
	{
		auto data = std::any_cast<Entity>(e.Data);
		Spawn(data);
	}
	else if (e.Type == GameEventType::VALUE_PLAYER_CHNG_POS_GRID)
	{
		auto& PLAYER_GRID_ENTITY = std::any_cast<Entity>(e.Data);

		m_PlayedGridPosition = PLAYER_GRID_ENTITY;

		for(auto& enemy : m_Enemies)
		{
			if (enemy->GetProperties().Intelligence > AI::DUMB)
			{
				enemy->Follow(Navigation::Navigate(enemy->GetLastPositionOnGrid(), m_PlayedGridPosition));
			}
		}
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
				SaveManager::AddScore(100);
			} else 
			{
				++it;
			}
		}
	}
	else if (e.Type == GameEventType::ENEMY_GRID_MOVEMENT)
	{
		Dispatch(GameEventType::ENEMY_GRID_MOVEMENT, e.Data);
	}
	else 
	{
		// BOMB_PLACED, BOMB_EXPLODED, BOMB_VANISHED
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

void EnemySpawner::CleanUp()
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
