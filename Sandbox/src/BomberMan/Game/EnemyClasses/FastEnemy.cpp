#include "FastEnemy.hpp"

FastEnemy::FastEnemy(Entity& handle, Entity& grid_entity) : Enemy(handle, grid_entity)
{
	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_FAST;

	m_Properties.Health = 2;
	m_Properties.Intelligence = AI::RANDOM;
	m_Properties.Name = "Bob";
	m_Properties.Speed = 3.0f;

	m_Animator->Add(ResourceManager::GetAnimation("AngryBallMoveAnimation"));
	m_Animator->Add(ResourceManager::GetAnimation("AngryBallDeadAnimation"));

	m_Animator->SetTarget(m_Handle);
	m_Animator->Play("AngryBallMove");
}

void FastEnemy::OnUpdateInternal(Timestep& ts)
{

}

void FastEnemy::OnGameEvent(GameEvent& e)
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
					m_Handle.GetComponent<Animator>().Play("AngryBallDead");
					break;
				}
				m_Hit = true;
			}
		}

	}
}

void FastEnemy::OnChangeDirection(Direction& dir)
{
	m_Animator->Play("AngryBallMove");
}

bool FastEnemy::EnemyLogic(Timestep& ts)
{
	if (m_Path.empty())
	{
		// No cooldown 
		Follow(Navigation::RandomPath(GetLastPositionOnGrid()));

		m_CurrentDirection = Direction::NONE;
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

