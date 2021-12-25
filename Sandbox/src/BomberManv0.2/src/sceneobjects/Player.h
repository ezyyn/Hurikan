#pragma once

#include "../core/FrameAnimator.h"

#include <Hurikan.h>
using namespace Hurikan;

struct PlayerData
{
	int Health = 3;

	float Speed = 5.0f;
	glm::vec2 Direction = {};
	glm::vec2 Velocity = {};
	bool Rotated = false;
};

class Player
{
public:
	Player() = default;
	~Player() = default;

	void Init();
	void OnUpdate(Timestep ts);
	void SetStartPosition(const glm::vec2& position) { m_Handle.Transform().Translation = { position.x, position.y, 0 }; }

	void Hit(int damage) { m_PlayerData.Health -= damage; }
	void FinalHit() { m_PlayerData.Health = 0; }

	void OnKeyPressed(KeyPressedEvent& e);
	void OnKeyReleased(KeyReleasedEvent& e);

	inline glm::vec2 Position() 
	{
		return { m_Handle.Transform().Translation.x, m_Handle.Transform().Translation.y };
	}

	inline glm::vec3 Position2()
	{
		return m_Handle.Transform().Translation;
	}

	inline glm::vec2 RoundPosition()
	{
		return { glm::round(m_Handle.Transform().Translation.x), glm::round(m_Handle.Transform().Translation.y) };
	}

public:
	bool m_Alive = true;
private:
	FrameAnimator m_PlayerAnimator;

	PlayerData m_PlayerData;

	TransformComponent* m_Transform = nullptr;
	Entity m_Handle;
};