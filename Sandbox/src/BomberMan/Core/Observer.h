#pragma once

#include <list>
#include <any>

enum class GameEventType : unsigned short
{
	BOMB_PLACED = 0, 
	VALUE_PLAYER_CHNG_POS_GRID, 
	VALUE_PLAYER_MOVING, 
	GAME_LOST, 
	BOMB_EXPLODED,
	BOMB_EXPLODING,
	BOMB_VANISHED, 
	VALUE_PLAYER_START_POS,
	BREAK_WALL,
	NEW_PATH_TO_PLAYER,
	CREATE_NEW_ENEMY,
	ENEMY_GRID_MOVEMENT,
	ENEMY_DEAD,
	DEPLOY_BOMB,
	DEPLOY_BOMB_UNSUCCESSFUL,

	PLAYER_IDLE,
	PLAYER_DIR_UP,
	PLAYER_DIR_DOWN,
	PLAYER_DIR_LEFT,
	PLAYER_DIR_RIGHT,

	SCORE_UPDATE,
	GAME_PAUSE,
	GAME_UNPAUSE,
	GAME_LOADED,

	NEW_GAME_CONFIRMED,
	CONTINUE_CONFIRMED,
	SETTINGS_CONFIRMED,
	SCORE_CONFIMED,
	EXIT_CONFIRMED,

	RETURN_TO_MAIN_MENU,

	SAVE_GAME,

	KEY_OBTAINED,
	GAME_WON,
	DISPLAY_KEY_FIRST
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
	void Dispatch(GameEventType type, const std::any& data = std::any());
private:
	std::list<Observer*> m_Observers;
};

