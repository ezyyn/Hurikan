#pragma once

#include <list>
#include <any>

enum class GameEventType : int
{
	BOMB_PLACED = 0, VALUE_PLAYER_CHNG_POS_GRID, VALUE_PLAYER_MOVING, PLAYER_DEAD, BOMB_EXPLODED, BOMB_VANISHED, VALUE_PLAYER_START_POS
};

struct GameEvent
{
	GameEvent(GameEventType type, const std::any& data)
		: Type(type), Data(data)
	{
	}

	GameEvent(const GameEvent& other)
	{
		Type = other.Type;
		Data = other.Data;
	}

	GameEventType Type;
	std::any Data;
};

class Observer
{
public:
	virtual void OnGameEvent(GameEvent& e) = 0;
};

class Observable
{
public:
	void Attach(Observer* obs);
	void Detach(Observer* obs);
protected:
	void DispatchToAll(GameEventType type, const std::any& data = std::any());
private:
	std::list<Observer*> m_Observers;
};

