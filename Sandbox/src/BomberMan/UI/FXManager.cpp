#include "FXManager.h"

void FXManager::Init(Scene* scene)
{
	m_CurrentScene = scene;
}
void FXManager::OnUpdate(Timestep& ts)
{
	auto& it = m_PlayingEffects.begin();

	while (it != m_PlayingEffects.end())
	{
		if ((*it)->Type == FXType::SCORE_100)
		{
			if (static_cast<Score100Effect*>(*it)->OnUpdate(ts))
			{
				it = m_PlayingEffects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void FXManager::OnGameEvent(GameEvent& e)
{
	if(e.Type == GameEventType::BREAK_WALL)
	{
		m_PlayingEffects.emplace_back(new Score100Effect(m_CurrentScene, FXType::SCORE_100, 3000,
			std::any_cast<Entity>(e.Data).Transform().Translation));
	}
}
