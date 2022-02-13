#pragma once


#include "BomberMan/Core/Observer.h"
#include "BomberMan/Core/Animator.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

struct PlayerData
{
	glm::vec2 Direction = { 0, 0 };
	glm::vec2 Velocity = { 0, 0 };
	bool Rotated = false;
	float Speed = 5.0f;
	unsigned int AvailableBombs = 5;
};

enum class PlayerAnimationState
{
	IDLE = 0, UP, DOWN, LEFT, RIGHT
};

class Hurikan::Scene;
class Player : public Observable, public Observer
{
public:
	void Create(Hurikan::Scene& scene);
	void OnUpdate(Timestep& ts);
	void OnGameEvent(GameEvent& e) override;
private:
	void OnUpdateMovement();
private:
	unsigned short m_PressedKey, m_LastKey;

	int m_Health = 1;
	bool m_Alive = true;

	PlayerData m_PlayerData;
	TransformComponent* m_PlayerTransform;

	Animator m_PlayerAnimator;

	PlayerAnimationState m_AnimationState;

	Entity m_Handle;
};