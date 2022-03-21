#pragma once


#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/Core/Animator.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

#define MIN_AVAILABLE_BOMBS 1
#define MAX_UPGRADE_COUNT_BOMBS 3
#define MIN_POWER 1
#define MIN_SPEED 4.6f

#define MAX_HEART 3

struct PlayerData
{
	glm::vec2 Direction = { 0, 0 };
	glm::vec2 Velocity = { 0, 0 };
	bool Rotated = false;
	float Speed = MIN_SPEED;
	int AvailableBombs = MIN_AVAILABLE_BOMBS;
	int Power = MIN_POWER;
	int Health = MAX_HEART;
	float Invincibility = 0.0f;
};

enum class PlayerAnimationState
{
	IDLE = 0, 
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player : public Observable, public Observer
{
public:
	Player() {}
	~Player() {}
	// Inicializuje entitu hráče
	void Create(Hurikan::Scene& scene);
	// Volaná funkce z OnUpdate funkce třídy InGame
	void OnUpdate(Timestep& ts);
private:
	// Funkce z Observeru
	void OnGameEvent(GameEvent& e) override;
	// Stará se o pohyb hráče
	void OnUpdateMovement();
private:
	// Potřebné proměnné pro stisknutí pro kontrolu mačkání tlačítek
	unsigned short m_PressedKey = 0, m_LastKey = 0; 
	bool m_BReleased = true; // Indikátor pro zamezení spamování bomby
	bool m_PlayerHit = false; // Indikuje jestli byl hráč zasažen nepřátelskou entitou
	bool m_GameOver = false; // Indikátor konce hry
	bool m_Paused = false; // Indikátor pozastavení hry
	glm::vec4 m_HitPulseColor = glm::vec4(1.0f); // Barva hráče během kolize s nepřátelskou entitou
	// Správce animací hráče
	Animator m_PlayerAnimator; 
	PlayerAnimationState m_AnimationState = PlayerAnimationState::IDLE; // Aktuální stav hráče
	PlayerData m_PlayerData; // Data o hráči
	Entity m_Handle; // Entita hráče
};