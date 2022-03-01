#pragma once


#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/Core/Animator.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

#define MIN_AVAILABLE_BOMBS 1
#define MAX_UPGRADE_COUNT_BOMBS 3
#define MIN_POWER 1
#define MIN_SPEED 4.0f

struct PlayerData
{
	glm::vec2 Direction = { 0, 0 };
	glm::vec2 Velocity = { 0, 0 };
	bool Rotated = false;
	float Speed = MIN_SPEED;
	int AvailableBombs = MIN_AVAILABLE_BOMBS;
	int Power = MIN_POWER;
	int Health = 3;
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
	void Create(Hurikan::Scene& scene);
	void OnUpdate(Timestep& ts);
private:
	void OnGameEvent(GameEvent& e) override;
	void OnUpdateMovement();
private:
	unsigned short m_PressedKey, m_LastKey;

	bool m_BReleased = true;
	
	bool m_PlayerHit = false;
	glm::vec4 m_HitPulseColor = glm::vec4(1.0f);

	PlayerData m_PlayerData;
	TransformComponent* m_PlayerTransform;

	Animator m_PlayerAnimator;

	PlayerAnimationState m_AnimationState;

	Entity m_Handle;
};