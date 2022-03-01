#include "FXManager.hpp"

#include "BomberMan/Game/Grid.hpp"

FXManager::~FXManager()
{
	if (m_PlayingEffects.empty())
		return;

	for (auto effect : m_PlayingEffects)
	{
		delete effect;
	}
}


void FXManager::Init(Scene* scene)
{
	m_CurrentScene = scene;
}
void FXManager::OnUpdate(Timestep& ts)
{
	auto& it = m_PlayingEffects.begin();

	while (it != m_PlayingEffects.end())
	{
		if ((*it)->OnUpdateInternal(ts))
		{
			delete *it;
			it = m_PlayingEffects.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void FXManager::OnGameEvent(GameEvent& e)
{
	if(/*e.Type == GameEventType::BREAK_WALL ||*/ e.Type == GameEventType::ENEMY_DEAD)
	{
		auto& data = std::any_cast<Entity>(e.Data);

		if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_REGULAR)
		{
			m_PlayingEffects.emplace_back(new Score100Effect(m_CurrentScene, 3000,
			std::any_cast<Entity>(e.Data).Transform().Translation));
		}
		else if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_FAST)
		{
			m_PlayingEffects.emplace_back(new Score300Effect(m_CurrentScene, 3000,
				std::any_cast<Entity>(e.Data).Transform().Translation));
		}
		/*else if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_BOSS)
		{
			m_PlayingEffects.emplace_back(new Score300Effect(m_CurrentScene, FXType::SCORE_300, 3000,
				std::any_cast<Entity>(e.Data).Transform().Translation));
		}*/
		else if (data.GetComponent<EntityTypeComponent>().Type == EntityType::ENEMY_SMART)
		{
			m_PlayingEffects.emplace_back(new Score500Effect(m_CurrentScene,3000,
				std::any_cast<Entity>(e.Data).Transform().Translation));
		}
	}
}
