#include "RareEnemy.h"

#include "BomberMan/Game/Grid.h"
#include "BomberMan/Core/Navigation.h"

#include "BomberMan/Core/ResourceManager.h"

RareEnemy::RareEnemy(Entity& handle, Entity& grid_entity) : Enemy(handle, grid_entity)
{
	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_REGULAR;

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));

	m_Properties.Health = 3;
	m_Properties.Intelligence = AI::DUMB;
	m_Properties.Name = "Bob";

	auto& fa = m_Handle.AddCustomComponent<Animator>();
	fa.Add(ResourceManager::GetAnimation("AngryBallMoveAnimation"));
	fa.Add(ResourceManager::GetAnimation("AngryBallDeadAnimation"));

	fa.SetTarget(m_Handle);
	fa.Play("AngryBallMove");
}

void RareEnemy::OnUpdate(Timestep& ts)
{
	auto& fa = m_Handle.GetComponent<Animator>();
	fa.OnUpdate(ts);

	if (m_Hit)
	{
		m_PulseColor.g = Utils::Lerp(m_PulseColor.g, 0.0f, ts * 2);
		m_PulseColor.b = Utils::Lerp(m_PulseColor.b, 0.0f, ts * 2);

		if (m_PulseColor.g == 0.0f)
		{
			m_Hit = false;
		} 
		fa.SetColor(m_PulseColor);
	} else 
	{
		if (m_PulseColor.g != 1.0f)
		{
			m_PulseColor.g = Utils::Lerp(m_PulseColor.g, 1.0f, ts * 2);
			m_PulseColor.b = Utils::Lerp(m_PulseColor.b, 1.0f, ts * 2);

			fa.SetColor(m_PulseColor);
		}
	}

	// Checking enemy's health
	if (!m_Alive)
	{
		if (!fa.IsAnyPlaying())
		{
			Dispatch(GameEventType::ENEMY_DEAD, m_Handle);
		}
		return;
	}

	Dispatch(GameEventType::ENEMY_GRID_MOVEMENT, m_Handle);

	if (EnemyLogic(ts))
	{
		// Movement and animation
		auto& transform = m_Handle.Transform();

		m_LastPositionOnGrid = m_Path.front();

		m_PreviousPosition = transform.Translation;

		transform.Translation.x = Utils::Lerp(m_Handle.Transform().Translation.x, m_Path.front().Transform().Translation.x, ts * 3);
		transform.Translation.y = Utils::Lerp(m_Handle.Transform().Translation.y, m_Path.front().Transform().Translation.y, ts * 3);

		if (m_PreviousPosition.x < transform.Translation.x && m_CurrentDirection != Direction::RIGHT)
		{
			m_CurrentDirection = Direction::RIGHT;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_IsRotated = true;
			m_Handle.GetComponent<Animator>().Play("AngryBallMove");
		}
		else if (m_PreviousPosition.x > transform.Translation.x && m_CurrentDirection != Direction::LEFT)
		{
			m_CurrentDirection = Direction::LEFT;
			if (!m_IsRotated)
				m_Handle.Transform().Scale.x *= -1;
			m_IsRotated = false;
			m_Handle.GetComponent<Animator>().Play("AngryBallMove");
		}

		if (m_PreviousPosition.y < transform.Translation.y && m_CurrentDirection != Direction::UP)
		{
			m_CurrentDirection = Direction::UP;
			m_IsRotated = false;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_Handle.GetComponent<Animator>().Play("AngryBallMove");
		}
		else if (m_PreviousPosition.y > transform.Translation.y && m_CurrentDirection != Direction::DOWN)
		{
			m_CurrentDirection = Direction::DOWN;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_IsRotated = false;
			m_Handle.GetComponent<Animator>().Play("AngryBallMove");
		}

		if (transform.Translation.x == m_Path.front().Transform().Translation.x && transform.Translation.y == m_Path.front().Transform().Translation.y)
		{
			Dispatch(GameEventType::ENEMY_GRID_MOVEMENT, m_Handle); // TODO: Replace with m_Path.front();
			m_Path.pop_front();
		}
	}
}

void RareEnemy::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::BOMB_EXPLODED || e.Type == GameEventType::BOMB_EXPLODING)
	{
		auto& spread = std::any_cast<std::list<Entity>>(e.Data);

		float range = 0.8f;
		if (e.Type == GameEventType::BOMB_EXPLODING)
			range = 0.5f;

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

const EnemyProps& RareEnemy::GetProperties()
{
	return m_Properties;
}

bool RareEnemy::EnemyLogic(Timestep& ts)
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
			m_FNRPC = rand() % 2 + 1.0f;
		}

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
