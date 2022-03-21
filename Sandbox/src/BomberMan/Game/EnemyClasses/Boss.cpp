#include "Boss.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Game/Grid.hpp"
#include "BomberMan/Core/Navigation.hpp"

Boss::Boss(Scene* scene, Entity& grid_entity) : Enemy(scene->CreateEntityWithDrawOrder(2), grid_entity)
{
	g_GameScene = scene;

	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_BOSS;

	m_Properties.Health = 3;
	m_Properties.Intelligence = AI::FOLLOW_RANGE;
	m_Properties.Name = "Bill";
	m_Properties.Speed = 6.0f;

	auto& animator = m_Handle.GetComponent<Animator>();
	animator.Add(ResourceManager::GetAnimation("BossIdleAnimation"));
	animator.Add(ResourceManager::GetAnimation("BossLeftAnimation"));
	animator.Add(ResourceManager::GetAnimation("BossUpAnimation"));
	animator.Add(ResourceManager::GetAnimation("BossDownAnimation"));
	animator.Add(ResourceManager::GetAnimation("BossDeadAnimation"));
	animator.SetTarget(m_Handle);
	animator.SetColor(glm::vec4(1.0f));

	m_Handle.GetComponent<Animator>().Play("BossIdle");
}

void Boss::OnUpdateInternal(Timestep& ts)
{
}

void Boss::OnChangeDirection(Direction& dir)
{
	if(dir == Direction::DOWN)
		m_Handle.GetComponent<Animator>().Play("BossDown");
	else if (dir == Direction::UP)
		m_Handle.GetComponent<Animator>().Play("BossUp");
	else if (dir == Direction::LEFT)
		m_Handle.GetComponent<Animator>().Play("BossLeft");
	else if (dir == Direction::RIGHT)
		m_Handle.GetComponent<Animator>().Play("BossLeft");
}

void Boss::OnGameEvent(GameEvent& e)
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
					m_Handle.GetComponent<Animator>().Play("BossDead");
					break;
				}
				m_Hit = true;
			}
		}
	}
	else if (e.Type == GameEventType::PLAYER_CHANGED_GRID_POSITION)
	{
		m_PlayersParent = std::any_cast<Entity>(e.Data);
	}
	else if (e.Type == GameEventType::CUTSCENE_COMPLETED)
	{
		m_CutSceneCompleted = true;
	}
}

bool Boss::EnemyLogic(Timestep& ts)
{
	if (!m_CutSceneCompleted)
	{
		m_Moving = false;
		return false;
	}

	if (!m_RecalculatePath)
	{
		m_RecalculatePathTimer -= ts;

		if (m_RecalculatePathTimer <= 0.0f)
		{
			auto path = Navigation::RandomPath(GetLastPositionOnGrid());
			Follow(path);
			m_RecalculatePath = true;
		}
		m_Moving = false;
		return false;
	}

	if (m_Path.empty() && m_RecalculatePath)
	{
		auto& path = Navigation::Navigate(GetLastPositionOnGrid(), m_PlayersParent);
		if (path.empty())
		{
			m_RecalculatePathTimer = 3.0f;
			m_RecalculatePath = false;
		}
		else
		{
			Follow(path);
		}
		m_Moving = false;
		return false;
	}
	else if(!m_Path.empty())
	{
		// Checking next node to make sure enemy monster does not bump into it
		if (m_Path.front().GetComponent<GridNodeComponent>().Obstacle)
		{
			auto path = Navigation::RandomPath(GetLastPositionOnGrid());

			if (!path.empty() && m_Path.front().GetComponent<EntityTypeComponent>().Type == EntityType::BOMB)
			{
				path = Navigation::RandomPath(GetLastPositionOnGrid());
			}
			Follow(path);
			m_Moving = false;
			return false;
		}
	}
	m_Moving = true;
	return true;
}
