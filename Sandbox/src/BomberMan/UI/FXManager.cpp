#include "FXManager.h"

FXManager::~FXManager()
{
	if (m_PlayingEffects.empty())
		return;

	for (auto effect : m_PlayingEffects)
	{
		delete static_cast<Score100Effect*>(effect);
	}
	m_PlayingEffects.clear();
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
		if ((*it)->Type == FXType::SCORE_100)
		{
			if (static_cast<Score100Effect*>(*it)->OnUpdate(ts))
			{
				delete static_cast<Score100Effect*>(*it);
				it = m_PlayingEffects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void FXManager::CleanUp()
{
	if (m_PlayingEffects.empty())
		return;

	for (auto effect : m_PlayingEffects)
	{
		delete static_cast<Score100Effect*>(effect);
	}
	m_PlayingEffects.clear();
}

void FXManager::OnGameEvent(GameEvent& e)
{
	if(/*e.Type == GameEventType::BREAK_WALL ||*/ e.Type == GameEventType::ENEMY_DEAD)
	{
		m_PlayingEffects.emplace_back(new Score100Effect(m_CurrentScene, FXType::SCORE_100, 3000,
			std::any_cast<Entity>(e.Data).Transform().Translation));
	}
}
