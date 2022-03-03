#include "Boss.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Game/Grid.hpp"
#include "BomberMan/Core/Navigation.hpp"

Boss::Boss(Scene* scene, Entity& grid_entity) : Enemy(scene->CreateEntityWithDrawOrder(2), grid_entity)
{
	g_GameScene = scene;

	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::ENEMY_BOSS;

	m_Handle.Transform().Scale *= 3;

	m_Properties.Health = 10;
	m_Properties.Intelligence = AI::FOLLOW_RANGE;
	m_Properties.Name = "Bill";
	m_Properties.Speed = 6.0f;

	m_Animator->Add(ResourceManager::GetAnimation("BossIdleAnimation"));
	
	m_Animator->SetTarget(m_Handle);
	m_Animator->SetColor(glm::vec4(1.0f));
	m_Animator->Play("BossIdle");
}

void Boss::OnUpdateInternal(Timestep& ts)
{
}

void Boss::OnChangeDirection(Direction& dir)
{
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
					//m_Handle.GetComponent<Animator>().Play("BlueIceCreamDead");
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
		return false;


	if (m_Path.empty())
	{
		auto& path = Navigation::Navigate(GetLastPositionOnGrid(), m_PlayersParent);
		Follow(path);
		return false;
	}
	else
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

			return false;
		}
	}

	return true;
}
