#include "Observer.hpp"

void Observable::Attach(Observer* obs)
{
	m_Observers.push_back(obs);
}
void Observable::Detach(Observer* obs)
{
	m_Observers.remove(obs);
}

void Observable::Dispatch(GameEventType type, const std::any& data/*= std::any()*/)
{
	auto& e = GameEvent(type, data);

	for (auto itr = m_Observers.begin(); itr != m_Observers.end(); ++itr)
	{
		(*itr)->OnGameEvent(e);
	}
}
