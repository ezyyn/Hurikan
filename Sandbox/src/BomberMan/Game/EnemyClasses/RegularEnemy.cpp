#include "RegularEnemy.hpp"

RegularEnemy::RegularEnemy(Entity& handle, Entity& grid_entity) : Enemy(handle, grid_entity)
{
	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_REGULAR;

	m_Properties.Health = 1;
	m_Properties.Intelligence = AI::RANDOM;
	m_Properties.Name = "Dingus";
	m_Properties.Speed = 2.0f;

	auto& animator = m_Handle.GetComponent<Animator>();

	animator.Add(ResourceManager::GetAnimation("BaloonMovingAnimation"));
	animator.Add(ResourceManager::GetAnimation("BaloonDeathAnimation"));

	animator.Play("BaloonMoving");
}

void RegularEnemy::OnUpdateInternal(Timestep& ts)
{
}

void RegularEnemy::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::BOMB_EXPLODED || e.Type == GameEventType::BOMB_EXPLODING)
	{
		auto& spread = std::any_cast<std::list<Entity>>(e.Data);

		constexpr float range = 0.8f;

		for (auto& explosion : spread)
		{
			if (glm::distance(m_Handle.Transform().Translation, explosion.Transform().Translation) < range)
			{
				if (--m_Properties.Health == 0)
				{
					m_Alive = false;
					m_Handle.GetComponent<Animator>().Play("BaloonDead");
					break;
				}
			}
		}
	}
}

void RegularEnemy::OnChangeDirection(Direction& dir)
{
	auto& animator = m_Handle.GetComponent<Animator>();
	animator.Play("BaloonMoving");
}

bool RegularEnemy::EnemyLogic(Timestep& ts)
{
	if (m_Path.empty())
	{
		// Cooldown for new random path
		if (m_FNRPC > 0.0f)
		{
			m_FNRPC -= ts;
		}

		if (m_FNRPC <= 0.0f)
		{
			Follow(Navigation::RandomPath(GetLastPositionOnGrid()));
			m_FNRPC = Utils::Random(2.0f, 4.0f);
		}

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

