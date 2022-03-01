#include "SmartEnemy.hpp"

#include "BomberMan/Core/Navigation.hpp"
#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Game/Grid.hpp"

SmartEnemy::SmartEnemy(Entity& handle, Entity& grid_entity) : Enemy(handle, grid_entity)
{
	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_SMART;

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));

	m_Properties.Health = 3;
	m_Properties.Intelligence = AI::SMART;
	m_Properties.Name = "George";
	m_Properties.Speed = 3.0f;

	auto& fa = m_Handle.AddCustomComponent<Animator>();
	fa.Add(ResourceManager::GetAnimation("BlueIceCreamMoveAnimation"));
	fa.Add(ResourceManager::GetAnimation("BlueIceCreamDeadAnimation"));

	fa.SetTarget(m_Handle);
	fa.Play("BlueIceCreamMove");
}

void SmartEnemy::OnUpdate(Timestep& ts)
{
	auto& fa = m_Handle.GetComponent<Animator>();
	fa.OnUpdate(ts);

	if (m_Hit)
	{
		m_HitColor.g = Utils::Lerp(m_HitColor.g, 0.0f, ts * 2);
		m_HitColor.b = Utils::Lerp(m_HitColor.b, 0.0f, ts * 2);

		if (m_HitColor.g == 0.0f)
		{
			m_Hit = false;
		}
		fa.SetColor(m_HitColor);
	}
	else
	{
		if (m_HitColor.g != 1.0f)
		{
			m_HitColor.g = Utils::Lerp(m_HitColor.g, 1.0f, ts * 2);
			m_HitColor.b = Utils::Lerp(m_HitColor.b, 1.0f, ts * 2);

			fa.SetColor(m_HitColor);
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
			m_Handle.GetComponent<Animator>().Play("BlueIceCreamMove");
		}
		else if (m_PreviousPosition.x > transform.Translation.x && m_CurrentDirection != Direction::LEFT)
		{
			m_CurrentDirection = Direction::LEFT;
			if (!m_IsRotated)
			{
				m_Handle.Transform().Scale.x *= -1;
				m_IsRotated = true;
			}
			m_Handle.GetComponent<Animator>().Play("BlueIceCreamMove");
		}

		if (m_PreviousPosition.y < transform.Translation.y && m_CurrentDirection != Direction::UP)
		{
			m_CurrentDirection = Direction::UP;
			m_IsRotated = false;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_Handle.GetComponent<Animator>().Play("BlueIceCreamMove");
		}
		else if (m_PreviousPosition.y > transform.Translation.y && m_CurrentDirection != Direction::DOWN)
		{
			m_CurrentDirection = Direction::DOWN;
			m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
			m_IsRotated = false;
			m_Handle.GetComponent<Animator>().Play("BlueIceCreamMove");
		}

		if (transform.Translation.x == m_Path.front().Transform().Translation.x && transform.Translation.y == m_Path.front().Transform().Translation.y)
		{
			Dispatch(GameEventType::ENEMY_MOVED, m_Handle);
			m_Path.pop_front();
		}
	}
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
			Follow(Navigation::RandomPath(GetLastPositionOnGrid()));

			return false;
		}
	}

	return true;
}
