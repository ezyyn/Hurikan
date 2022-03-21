#pragma once

#include "BomberMan/Game/GCamera.hpp"
#include "BomberMan/Game/Grid.hpp"
#include "BomberMan/Game/Player.hpp"
#include "BomberMan/Game/Bomb.hpp"
#include "BomberMan/Game/Enemy.hpp"
#include "BomberMan/UI/FXManager.hpp"
#include "BomberMan/UI/SimpleUI.hpp"

#include <Hurikan/Scene/Scene.h>

using namespace Hurikan;

class AudioAssistant;

class InGame : public Observer, public Observable
{
public:
	InGame() {};
	~InGame() {};
	// Inicializace všech modůlu třídy InGame
	// Třída AudioAssistant je sdílena se třídou GameManager
	void Init(AudioAssistant& assistant);
	// Voláná funkcí OnUpdate ve tříde GameManager
	void OnUpdate(Timestep& ts);
	// Zděděná funkce ze třídy Observer
	void OnGameEvent(GameEvent& e) override;
	// Funkce pro pozastavení hry.
	void Pause(bool pause);
	bool Paused() const { return m_Paused; }
private:
	// Třída Scene pochází z Hurikan enginu, je součástí entity komponent systému.
	// Stará se všechno renderování a simulaci fyziky.
	Hurikan::Scene m_InGameScene;
	GCamera m_GameCamera; // Herní kamera, která se pohybuje s hráčem
	Grid m_Grid; // Herní pole, kde se odehrává celá hra
	Player m_Player; // Správa hráče a jeho pohybu
	BombManager m_BombManager; // Správa bomb a jejich explozí
	EnemySpawner m_EnemySpawner; // Správa všech nepřátelských entit 
	FXManager m_FXManager; // Správce všech grafických efektů
	SimpleUI m_SimpleUI; // Jednoduché grafické uživatelské rozhraní 

	bool m_Paused = false; // Indikátor pozastavení hry
};