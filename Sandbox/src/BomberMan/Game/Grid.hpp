#pragma once

#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/Core/Utils.hpp"

#include <Hurikan/Core/Ref.h>
#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;


enum class EntityType 
{
	PLAYER = 0, BOMB, EMPTY, WALL,BREAKABLE_WALL, 
	ENEMY_REGULAR, ENEMY_FAST,ENEMY_SMART, ENEMY_BOSS,
};

struct EntityTypeComponent 
{
	EntityType Type;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(const EntityType& type) : Type(type) {}
};

enum class Loot
{
	EXIT = 0, // requires killing all of the monsters
	BOMB_UPGRADE_POWER,
	BOMB_UPGRADE_COUNT,
	SPEED_UPGRADE,
	HEALTH_POINT,
	KEY
};

struct LootComponent
{
	Loot Type;
	Entity LootHandle;
	bool Obtainable = false;

	LootComponent() = default;
	LootComponent(const LootComponent&) = default;
	LootComponent(const Loot& type) : Type(type) {}
};

// Strukturu používá A-Star algoritmus implementovaný v souboru Navigation.h
struct GridNodeComponent
{
	GridNodeComponent() = default;
	GridNodeComponent(const GridNodeComponent&) = default;

	bool Visited = false; // Indikuje jestli bylo dané políčko zpracované algoritmem
	bool Obstacle = false; // Indikuje jestli je dané políčko překážka
	float GlobalGoal = 0.0f; // Proměnná určující vzdálenost od globálního cíle
	float LocalGoal = 0.0f; // Proměnná určující vzdálenost od lokálního cíle
	glm::ivec2 Index = {}; // Zde jsou uloženy indexy políčka, protože jeho pozice neodpovída jeho pozici v poli.
	std::vector<Entity> Neighbours; // Zde jsou uloženy entity sousedů políčka
	Entity Parent; // Toto políčko se vždy mění a značí náváznost na předchozí políčko vybráné algoritmem
};

class Grid : public Observer, public Observable
{ 
public:
	Grid() {};
	~Grid();
	// Vytvoří pole entit pro daný level
	void Create(Scene* scene);
	// Pomocná funkce pro získání konkrétního políčka v poli.
	static Entity& Get(unsigned int y, unsigned int x);
	// Pomocné funkce pro získání výšky a šírky aktuálního pole. 
	static const int GetLevelHeight();
	static const int GetLevelWidth();
	// Slouží k pročistění GridNode komponentu, k této funkci se vrátím později.
	static void ClearNodes();
	// Opět funkce ze třídy Observer
	virtual void OnGameEvent(GameEvent& e) override;
	// Volaná funkce z OnUpdate funkce ve třídě InGame
	void OnUpdate(Timestep& ts);
private:
	// Statická definice pole entit z důvodu nutnosti zpřístupnení ostatním třídám bez 
	// nutnosti předání instance.
	static Entity* m_Grid;
	// Vektor obsahující právě probíhacíjí animace
	std::vector<Entity> m_AnimationQueue;
	// Indikátor převzetí klíče hráčem
	bool m_KeyObtained = false;
	// Indikátor konce hry
	bool m_GameOver = false;
	// Entita políčka, kde se momentálně nachází hráč.
	Entity m_PlayerGridPosition;
	// Entita klíče
	Entity m_KeyGridEntity; // Entita klíče
	// Paměťová adresa herní scény
	Scene* g_GameScene = nullptr;
};