#include "PlayerScript.h"

#include "FrameAnimatorScript.h"

#include "../core/AudioManager.h"

void PlayerScript::OnCreate()
{
	m_Stats = &GetComponent<PlayerStats>();
	m_Transform = &GetComponent<TransformComponent>();
	m_PlayerBody = (b2Body*)GetComponent<Rigidbody2DComponent>().RuntimeBody;
	m_FrameAnimator = &GetComponent<FrameAnimator>();
}

void PlayerScript::OnDestroy()
{
}

extern float pms;
float timer1 = pms;
float timer2 = 0;

void PlayerScript::OnUpdate(Timestep ts)
{
	// Animation
	m_FrameAnimator->OnUpdate(ts);

	if (m_Stats->Health <= 0)
		return;

	// Movement

	Direction = { 0, 0 };

	if (Input::IsKeyPressed(Key::W))
	{
		Direction.y = 1;
		m_IsRotated = false;
		m_Transform->Scale.x = glm::abs(GetComponent<TransformComponent>().Scale.x);

		if (m_FrameAnimator->GetActiveAnimation().Tag != "UpAnimation")
			m_FrameAnimator->Switch("UpAnimation");
	}
	if (Input::IsKeyPressed(Key::D))
	{
		timer1 += ts;
		if (timer1 >= pms)
		{
			timer1 = 0;
		//	AudioManager::Play(AudioType::PLAYER_HORIZONTAL_MOVEMENT);
		}

		Direction.x = 1;
		if (!m_IsRotated)
		{
			m_IsRotated = true;
			m_Transform->Scale.x *= (-1);
		}

		if (m_FrameAnimator->GetActiveAnimation().Tag != "LeftAnimation")
			m_FrameAnimator->Switch("LeftAnimation");
	}

	if (Input::IsKeyPressed(Key::S))
	{
		Direction.y = -1;
		m_IsRotated = false;
		m_Transform->Scale.x = glm::abs(GetComponent<TransformComponent>().Scale.x);

		if (m_FrameAnimator->GetActiveAnimation().Tag != "DownAnimation")
		{
			m_FrameAnimator->Switch("DownAnimation");
		}
	}
	if (Input::IsKeyPressed(Key::A))
	{
		timer1 += ts;
		if (timer1 >= pms)
		{
			timer1 = 0;
		//	AudioManager::Play(AudioType::PLAYER_HORIZONTAL_MOVEMENT);
		}

		Direction.x = -1;
		m_IsRotated = false;
		m_Transform->Scale.x = glm::abs(GetComponent<TransformComponent>().Scale.x);
		
		if (m_FrameAnimator->GetActiveAnimation().Tag != "LeftAnimation")
			m_FrameAnimator->Switch("LeftAnimation");
	}

	if (Direction == b2Vec2(0, 0))
	{
		m_FrameAnimator->Stop();
	}

	Velocity.x = Direction.x * m_Stats->Speed;
	Velocity.y = Direction.y * m_Stats->Speed;

	m_PlayerBody->SetLinearVelocity(Velocity);
}

