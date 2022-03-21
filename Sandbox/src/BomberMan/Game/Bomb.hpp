#pragma once

#include "BomberMan/Core/Observer.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

enum class BombState : unsigned short
{
	SET = 0, TICKING, EDGE, EXPLOSION, DONE
};

enum class BombType
{
	CLASSIC = 0,
	LANDMINE,
	DYNAMITE
};

struct BombProps
{
	BombType Type = BombType::CLASSIC;
	float ExplosionTime = 2; // in seconds 
	int Reach = 2;
private:
	Entity GridEntity;
	BombState State = BombState::SET;
	float Time = 0;

	friend class BombManager;
	friend class Bomb;
};

class Bomb : public Observable
{
public:
	// Inicializace bomby a její entity
	Bomb(const BombProps& props, Scene* scene);
	// Funkce volající se každý obrazový snímek
	bool OnTick(Timestep& ts);
public:
	// Pomocné funkce
	inline const glm::vec3& GetPosition() { return m_Handle.Transform().Translation; }
	inline const BombState& GetState() { return m_Properties.State; }
	inline Entity& GetParent() { return m_Parent; }
	inline bool PhysicsEnabled() { return m_PhysicsEnabled; }
	// Zapnutí fyzikálního objektu pro zapnutí kolíze mezi hráčem a bombou 
	void EnablePhysics();
private:
	// Bomba vybuchne
	void Explode();
	// Zde se výbuch rozšíří do okolí do křídel 
	void Expand();
	// Inicializování jednotlivých křídel výbuchu
	bool WingExpand(int index, Entity& entity, float rotation);
	// Zničení bomby
	void DestroyItSelf();
	// Pomocné operátory k porovnávání bomb
	inline bool operator==(Bomb& other) { return m_Handle == other.m_Handle; }
	inline bool operator!=(Bomb& other) { return !(*this == other); }
private:
	std::list<Entity> m_SpreadEntities; // List obsahující všechny entity exploze
	bool m_ChainExplosion = false; // Indikátor řetězového výbuchu
	int m_AnimationCompletedCount = 0; // Indikátor ukončení animací
	bool m_PhysicsEnabled = false; // Indikátor stavu fyziky

	BombProps m_Properties; // Vlastnosti bomby
	Entity m_Parent; // Entita políčka, na ktéré bomba leží
	Entity m_Handle; // Entita bomby
private:
	Scene* g_GameScene = nullptr; // Aktuální scéna
};

class BombManager : public Observer, public Observable
{
public:
	BombManager() {}
	~BombManager();
	// Pouze předává adresu paměty aktuální scény
	void Init(Scene* scene);
	// Funkce z Observeru
	void OnGameEvent(GameEvent& e) override;
	// Funkce volaná OnUpdate funkcí v třídě InGame
	void OnUpdate(Timestep& ts);
private:
	// Položí bombu a nastaví ji paramentry specifikované v argumentu
	// Vrací hodnotu true, jestli na daném políčku neleží již jiná bomba
	bool PlaceBomb(BombProps& props);
private:
	Scene* g_GameScene = nullptr;
	// Entita políčka na kterém zrovna stojí hráč
	Entity m_PlayerGrid;
};