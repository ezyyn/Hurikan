#include "SmartEnemy.hpp"

#include <Hurikan/Core/Log.h>

SmartEnemy::SmartEnemy(Entity& handle, Entity& grid_entity) : Enemy(handle, grid_entity)
{
	/*Level level;*/

	/*LevelLoader::GetLevelWithGeneratedMap(level);

	HU_INFO(level.MapSkeleton);*/

	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_SMART;

	m_Properties.Health = 3;
	m_Properties.Intelligence = AI::FOLLOW_RANGE;
	m_Properties.Name = "George";
	m_Properties.Speed = 3.0f;

	auto& animator = m_Handle.GetComponent<Animator>();

	animator.Add(ResourceManager::GetAnimation("BlueIceCreamMoveAnimation"));
	animator.Add(ResourceManager::GetAnimation("BlueIceCreamDeadAnimation"));

	animator.Play("BlueIceCreamMove");
}

void SmartEnemy::OnUpdateInternal(Timestep& ts)
{
}

void SmartEnemy::OnChangeDirection(Direction& dir)
{
	auto& animator = m_Handle.GetComponent<Animator>();
	animator.Play("BlueIceCreamMove");
}

void SmartEnemy::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::BOMB_EXPLODED || e.Type == GameEventType::BOMB_EXPLODING)
	{
		if (m_Hit)
			return;

		auto& spread = std::any_cast<std::list<Entity>>(e.Data);

		constexpr float range = 0.8f;

		for (auto& explosion : spread)
		{
			if (glm::distance(m_Handle.Transform().Translation, explosion.Transform().Translation) < range)
			{
				if (--m_Properties.Health == 0)
				{
					m_Alive = false;
					m_Handle.GetComponent<Animator>().Play("BlueIceCreamDead");
					break;
				}
				m_Hit = true;
				break;
			}
		}
	}
	else if (e.Type == GameEventType::PLAYER_MOVED)
	{
		m_PlayerPosition = std::any_cast<glm::vec3>(e.Data);
	}
	else if (e.Type == GameEventType::PLAYER_CHANGED_GRID_POSITION)
	{
		m_PlayersParent = std::any_cast<Entity>(e.Data);
	}
}

bool SmartEnemy::EnemyLogic(Timestep& ts)
{
	m_InRadius = glm::distance(m_Handle.Transform().Translation, m_PlayerPosition) < m_FollowRadius;

	if (!m_RecalculatePath) 
	{
		m_RecalculateTimer -= ts;

		if (m_RecalculateTimer <= 0.0f) 
		{
			m_RecalculateTimer = 3.0f;
			m_RecalculatePath = true;
		}
	}

	if (m_Path.empty())
	{
		// No cooldown 

		if (m_InRadius && m_RecalculatePath)
		{
			m_RecalculatePath = false;
			auto& path = Navigation::Navigate(GetLastPositionOnGrid(), m_PlayersParent);

			m_FollowsPlayer = !path.empty();
			if (m_FollowsPlayer)
				Follow(path);
			else
				Follow(Navigation::RandomPath(GetLastPositionOnGrid()));
		}
		else 
			Follow(Navigation::RandomPath(GetLastPositionOnGrid()));

		return false;
	}
	else
	{
		// Checking next node to make sure enemy monster does not bump into it
		if (m_Path.front().GetComponent<GridNodeComponent>().Obstacle)
		{
			// Find new random path

			auto path = Navigation::RandomPath(GetLastPositionOnGrid());

			if (!path.empty() && m_Path.front().GetComponent<EntityTypeComponent>().Type == EntityType::BOMB)
			{
				path = Navigation::RandomPath(GetLastPositionOnGrid());
			}
			Follow(path);

			return false;
		}
	}

	return true;
}
