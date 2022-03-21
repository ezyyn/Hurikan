/*
 * This is the observer design pattern.
 */

#pragma once

#include <list>
#include <any>

/*
 * List of all events happening in this game
 */

enum class GameEventType : unsigned short
{
	BOMB_PLACED = 0,
	BOMB_EXPLODED,
	BOMB_EXPLODING,
	BOMB_VANISHED,
	BOMB_DEPLOY,
	BOMB_DEPLOY_UNSUCCESSFUL,

	PLAYER_CHANGED_GRID_POSITION,
	PLAYER_MOVED,
	PLAYER_START_POSITION,
	PLAYER_SUCCESS_EXIT,
	PLAYER_GONE,
	PLAYER_HIT,
	PLAYER_IDLE,
	PLAYER_DIR_UP,
	PLAYER_DIR_DOWN,
	PLAYER_DIR_LEFT,
	PLAYER_DIR_RIGHT,
	PLAYER_POWER_UPGRADE,
	PLAYER_BOMB_COUNT_UPGRADE,
	PLAYER_SPEED_UPGRADE,
	PLAYER_ADD_HEART,

	ENEMY_CREATE_NEW,
	ENEMY_MOVED,
	ENEMY_DEAD,

	WALL_BREAK,

	GAME_NEW,
	GAME_CONTINUE,
	GAME_PAUSED,
	GAME_UNPAUSED,
	GAME_EXIT,
	GAME_COMPLETED,

	RETURN_TO_MAIN_MENU_CONFIRMED,
	RETURN_TO_MAIN_MENU,
	RETURN_FROM_MM_SETTINGS,

	KEY_OBTAINED,
	LEVEL_SUCCESS,
	LEVEL_FAILED,

	CUTSCENE_COMPLETED,

	AUDIO_LEVEL_SUCCESS,
	AUDIO_LEVEL_FAILED,
	AUDIO_LEVEL_SCREEN,
	AUDIO_MAINMENU,
	AUDIO_INGAME_LOOP,
	AUDIO_BOSS_LOOP,
};
/*
 * This struct just combines the actual event that has happened and a feature (std::any) that can store literally anyhing.
 * std::any is just a safe variant of a void pointer
 */
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

// Dědíme když chceme, aby daná třída byla schopna odposlouchávat komunikaci.
class Observer
{
private:
	// Zavolá se pokaždé, když třída Observable zavolá funkci Dispatch.
	virtual void OnGameEvent(GameEvent& e) = 0;

	friend class Observable;
};

// Dědíme když víme, že tato třída bude vyvolávat události, na které musí zareagovat ostatní třídy.
class Observable
{
public:
	// Přídaní posluchače událostí
	void Attach(Observer* obs);
	// Odebrání posluchače událostí
	void Detach(Observer* obs);
protected:
	// Třída dědící z této třídy má k dispozici tuto funkci pro rozhlášení všem přidaným posluchačum, že 
	// se stala nějaká událost.
	// GameEventType specifikuje, jaká událost se právě stala. Data jsou volitelný argument, lze 
	// tedy i poslat důležitá data.
	void Dispatch(GameEventType type, const std::any& data = std::any());
private:
	std::list<Observer*> m_Observers;
};

