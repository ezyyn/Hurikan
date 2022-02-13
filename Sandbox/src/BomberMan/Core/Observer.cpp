#include "Observer.h"

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
	for (auto itr = m_Observers.begin(); itr != m_Observers.end(); ++itr)
	{
		(*itr)->OnGameEvent(GameEvent(type, data));
	}
}
