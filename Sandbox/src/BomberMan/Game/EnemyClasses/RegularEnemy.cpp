#include "RegularEnemy.hpp"

#include "BomberMan/Game/Grid.hpp"

#include "BomberMan/Core/Navigation.hpp"
#include "BomberMan/Core/ResourceManager.hpp"

RegularEnemy::RegularEnemy(Entity& handle, Entity& grid_entity) : Enemy(handle, grid_entity)
{
	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_REGULAR;

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));

	m_Properties.Health = 1;
	m_Properties.Intelligence = AI::RANDOM;
	m_Properties.Name = "Dingus";
	m_Properties.Speed = 2.0f;

	auto& fa = m_Handle.AddCustomComponent<Animator>();
	fa.Add(ResourceManager::GetAnimation("BaloonMovingAnimation"));
	fa.Add(ResourceManager::GetAnimation("BaloonDeathAnimation"));

	fa.SetTarget(m_Handle);
	//fa.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	fa.Play("BaloonMoving");
}

void RegularEnemy::OnUpdate(Timestep& ts)
{
	auto& fa = m_Handle.GetComponent<Animator>();
	fa.OnUpdate(ts);
	
	// Checking enemy's health
	if (!m_Alive)
	{
		if(!fa.IsAnyPlaying())
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
			m_Handle.GetComponent<Animator>().Play("BaloonMoving");
		}
		else if (m_PreviousPosition.x > transform.Translation.x && m_CurrentDirection != Direction::LEFT)
		{
			m_CurrentDirection = Direction::LEFT;
			if (!m_IsRotated)
			{
				m_Handle.Transform().Scale.x *= -1;
				m_IsRotated = true;
			}
			m_Handle.GetComponent<Animator>().Play("BaloonMoving");
		}

		if (m_PreviousPosition.y < transform.Translation.y && m_CurrentDirection != Direction::UP)
		{
			m_CurrentDirection = Direction::UP;
			m_IsRotated = false;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_Handle.GetComponent<Animator>().Play("BaloonMoving");
		}
		else if (m_PreviousPosition.y > transform.Translation.y && m_CurrentDirection != Direction::DOWN)
		{
			m_CurrentDirection = Direction::DOWN;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_IsRotated = false;
			m_Handle.GetComponent<Animator>().Play("BaloonMoving");
		}
		
		if (transform.Translation.x == m_Path.front().Transform().Translation.x && transform.Translation.y == m_Path.front().Transform().Translation.y)
		{
			m_Path.pop_front();
		}
	}
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
				// Play hit animation 
			}
		}
	}
	else if (e.Type == GameEventType::BOMB_PLACED)
	{
	}
	else if (e.Type == GameEventType::BOMB_VANISHED)
	{
	}
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
			Follow(Navigation::RandomPath(m_LastPositionOnGrid));
			m_FNRPC = rand() % (4 - 2) + 2.0f;
		}

		//m_Handle.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("PlayerIdle");
		//m_Handle.GetComponent<Animator>().Pause();
		m_CurrentDirection = Direction::NONE;
		return false;
	} 
	else 
	{
		// Checking next node to make sure enemy monster does not bump into it
		if (m_Path.front().GetComponent<GridNodeComponent>().Obstacle)
		{
			m_Path.clear();

			// Find new random path
			Follow(Navigation::RandomPath(m_LastPositionOnGrid));
			return false;
		}
	}
	
	return true;
}
