#include "Player.h"

#include "BomberMan/Core/ResourceManager.h"

#include <box2d/b2_body.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>

#include <iostream>

void Player::Create(Hurikan::Scene& scene)
{
	m_Handle = scene.CreateEntityWithDrawOrder(3, "Player");
	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("PlayerIdle");
	// PLAYER ANIMATIONS
	m_PlayerAnimator.SetDebugTag("Player");
	m_PlayerAnimator.SetTarget(m_Handle);
	//
	m_PlayerAnimator.Add(ResourceManager::GetAnimation("PlayerLeftAnimation"));
	m_PlayerAnimator.Add(ResourceManager::GetAnimation("PlayerUpAnimation"));
	m_PlayerAnimator.Add(ResourceManager::GetAnimation("PlayerDownAnimation"));
	m_PlayerAnimator.Add(ResourceManager::GetAnimation("PlayerDeadAnimation"));

	///////////////////////////////////////////////////////////////////
	////////////////////     Player Physics     ///////////////////////
	///////////////////////////////////////////////////////////////////
	auto& rb2d = m_Handle.AddComponent<Rigidbody2DComponent>();
	rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rb2d.Gravity = false;
	rb2d.FixedRotation = true;

	float scale = 0.96f;
	auto& scale_cmp = m_Handle.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

	auto& cc2d = m_Handle.AddComponent<CircleCollider2DComponent>();
	cc2d.Radius = scale / 2.0f;
	cc2d.Friction = 0.0f;
	cc2d.IsSensor = false;

	m_PlayerTransform = &m_Handle.Transform();
}

void Player::OnUpdate(Timestep& ts)
{
	m_PlayerAnimator.OnUpdate(ts);
	OnUpdateMovement();
}

void Player::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::VALUE_PLAYER_START_POS)
	{
		//HU_INFO("{0}", std::any_cast<glm::vec3>(e.Data).x);
		m_Handle.Transform().Translation = std::any_cast<glm::vec3>(e.Data);
	}
	else if (e.Type == GameEventType::BOMB_EXPLODED)
	{
		// Check if the player is dead or not
		auto& explosion = std::any_cast<std::list<Entity>>(e.Data);

		for (auto& exp : explosion)
		{
			auto& player = m_Handle.Transform().Translation;
			auto& expl = exp.Transform().Translation;

			HU_INFO("{0}", glm::distance(player, expl));

			if (glm::distance(player, expl) < 1.0f)
			{
				HU_INFO("PLAYER HIT!")

				// Player dead animation
			}
		}
	}
}

#define SIPKY

#ifdef SIPKY

#define kkey_up Key::Up
#define kkey_down Key::Down
#define kkey_left Key::Left
#define kkey_right Key::Right

#else

#define kkey_up Key::W
#define kkey_down Key::S
#define kkey_left Key::A
#define kkey_right Key::D

#endif

void Player::OnUpdateMovement()
{
	m_PlayerData.Direction = { 0, 0 };

	if (Input::IsKeyPressed(kkey_up))
	{
		m_PlayerData.Direction.y = 1;
		m_PlayerData.Rotated = false;
		m_PlayerTransform->Scale.x = glm::abs(m_PlayerTransform->Scale.x);
		m_PressedKey = kkey_up;
	}

	if (Input::IsKeyPressed(kkey_right))
	{
		m_PlayerData.Direction.x = 1;
		if (!m_PlayerData.Rotated)
		{
			m_PlayerData.Rotated = true;
			m_PlayerTransform->Scale.x *= (-1);
		}
		m_PressedKey = kkey_right;
	}

	if (Input::IsKeyPressed(kkey_down))
	{
		m_PlayerData.Direction.y = -1;
		m_PlayerData.Rotated = false;
		m_PlayerTransform->Scale.x = glm::abs(m_Handle.GetComponent<TransformComponent>().Scale.x);
		m_PressedKey = kkey_down;
	}

	if (Input::IsKeyPressed(kkey_left))
	{
		m_PlayerData.Direction.x = -1;
		m_PlayerData.Rotated = false;
		m_PlayerTransform->Scale.x = glm::abs(m_Handle.GetComponent<TransformComponent>().Scale.x);
		m_PressedKey = kkey_left;
	}

	if (m_PressedKey != m_LastKey)
		switch (m_PressedKey)
		{
		case kkey_up:
		{
			m_AnimationState = PlayerAnimationState::UP;
			m_PlayerAnimator.Play("UpAnimation");
			DispatchToAll(GameEventType::PLAYER_DIR_UP);
			m_LastKey = kkey_up;
			break;
		}
		case kkey_down:
		{
			m_AnimationState = PlayerAnimationState::DOWN;
			m_PlayerAnimator.Play("DownAnimation");
			DispatchToAll(GameEventType::PLAYER_DIR_DOWN);

			m_LastKey = kkey_down;
			break;
		}
		case kkey_left:
		{
			m_AnimationState = PlayerAnimationState::LEFT;
			m_PlayerAnimator.Play("LeftAnimation");
			DispatchToAll(GameEventType::PLAYER_DIR_LEFT);

			m_LastKey = kkey_left;
			break;
		}
		case kkey_right:
		{
			m_AnimationState = PlayerAnimationState::RIGHT;
			m_PlayerAnimator.Play("LeftAnimation");
			DispatchToAll(GameEventType::PLAYER_DIR_RIGHT);

			m_LastKey = kkey_right;
			break;
		}
		default:
		{
			m_PlayerData.Velocity.x = 0; // ?
			m_PlayerData.Velocity.y = 0;
			break;
		}
		}

	if (m_PlayerData.Direction.x == 0 && m_PlayerData.Direction.y == 0)
	{
		m_AnimationState = PlayerAnimationState::IDLE;
		DispatchToAll(GameEventType::PLAYER_IDLE);
		m_Handle.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("PlayerIdle");
		m_PlayerAnimator.Pause();
		m_LastKey = 0;
		m_PressedKey = 0;
	}

	m_PlayerData.Velocity.x = m_PlayerData.Direction.x * m_PlayerData.Speed;
	m_PlayerData.Velocity.y = m_PlayerData.Direction.y * m_PlayerData.Speed;

	auto body = static_cast<b2Body*>(m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody);

	if (glm::abs(m_PlayerData.Velocity.x) == 1 && glm::abs(m_PlayerData.Velocity.y) == 1)
	{
		switch (m_LastKey)
		{
		case Key::W:
		case Key::S:
			m_PlayerData.Velocity.x = 0;
			break;
		case Key::A:
		case Key::D:
			m_PlayerData.Velocity.y = 0;
			break;
		default:
			break;
		}
	}

	// Dispatch to all observers that player has moved
	if (m_PlayerData.Velocity.x || m_PlayerData.Velocity.y)
	{
		DispatchToAll(GameEventType::VALUE_PLAYER_MOVING, m_Handle.Transform().Translation);
	}

	body->SetLinearVelocity({ m_PlayerData.Velocity.x, m_PlayerData.Velocity.y });
}

