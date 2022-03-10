#include "Player.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Game/Bomb.hpp"
#include "BomberMan/Core/Utils.hpp"

#include <box2d/b2_body.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>

extern GameData g_InGameData;

void Player::Create(Hurikan::Scene& scene)
{
	m_PlayerData.AvailableBombs += g_InGameData.BombCountUpgrade;
	m_PlayerData.Speed += g_InGameData.SpeedUpgrade;
	m_PlayerData.Power += g_InGameData.BombPowerUpgrade;

	m_Handle = scene.CreateEntityWithDrawOrder(3, "Player");
	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
	// PLAYER ANIMATIONS
	m_PlayerAnimator.SetDebugTag("Player");
	m_PlayerAnimator.SetTarget(m_Handle);
	
	m_PlayerAnimator.Add(ResourceManager::GetAnimation("PlayerIdleAnimation"));
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

	constexpr float scale = 0.96f;
	auto& scale_cmp = m_Handle.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

	auto& cc2d = m_Handle.AddComponent<CircleCollider2DComponent>();
	cc2d.Radius = scale / 2.0f;
	cc2d.Friction = 0.0f;
	cc2d.IsSensor = false;

	m_PlayerTransform = &m_Handle.Transform();
	m_PlayerAnimator.Play("PlayerIdle");
}

void Player::OnUpdate(Timestep& ts)
{
	m_PlayerAnimator.OnUpdate(ts);

/* DEBUG
	static bool r = false;

	if (!r && Input::IsKeyPressed(Key::T))
	{
		r = true;
		m_PlayerHit = true;
	}
	if (Input::IsKeyReleased(Key::T))
	{
		r = false;
	}*/

	if (m_PlayerData.Health != 0)
	{
		if (m_PlayerHit) {

			m_HitPulseColor.g = Utils::Lerp(m_HitPulseColor.g, 0.5f, ts * 2);
			m_HitPulseColor.b = Utils::Lerp(m_HitPulseColor.b, 0.5f, ts * 2);
			m_PlayerAnimator.SetColor(m_HitPulseColor);

			if (m_HitPulseColor.b == 0.5f) {
				m_PlayerHit = false;
			}
		}
		else
		{
			if (m_HitPulseColor.g != 1.0f)
			{
				m_HitPulseColor.g = Utils::Lerp(m_HitPulseColor.g, 1.0f, ts * 2);
				m_HitPulseColor.b = Utils::Lerp(m_HitPulseColor.b, 1.0f, ts * 2);

				m_PlayerAnimator.SetColor(m_HitPulseColor);
			}
		}

		if (m_GameOver)
			return;

		if (m_BReleased && Input::IsKeyPressed(Key::B))
		{
			m_BReleased = false;

			if (m_PlayerData.AvailableBombs)
			{
				BombProps props;
				props.ExplosionTime = 3.0f;
				props.Reach = m_PlayerData.Power;
				props.Type = BombType::CLASSIC;

				Dispatch(GameEventType::BOMB_DEPLOY, props);
				m_PlayerData.AvailableBombs--;
			}
		}

		if (m_PlayerData.Invincibility != 0.0f)
		{
			m_PlayerData.Invincibility = Utils::Lerp(m_PlayerData.Invincibility, 0.0f, ts);
		}
		m_BReleased = Input::IsKeyReleased(Key::B);
		OnUpdateMovement();
	}
}

void Player::OnGameEvent(GameEvent& e)
{
	if (!m_PlayerData.Health)
		return;

	if (e.Type == GameEventType::PLAYER_START_POSITION)
	{
		//HU_INFO("{0}", std::any_cast<glm::vec3>(e.Data).x);
		m_Handle.Transform().Translation = std::any_cast<glm::vec3>(e.Data);
	}
	else if (e.Type == GameEventType::BOMB_EXPLODED || e.Type == GameEventType::BOMB_EXPLODING)
	{
		float range = 0.8f;
		if (e.Type == GameEventType::BOMB_EXPLODING)
			range = 0.5f;

		if (e.Type == GameEventType::BOMB_EXPLODED)
		{
			m_PlayerData.AvailableBombs++;
		}

		// Check if the player is dead or not
		auto& explosion = std::any_cast<std::list<Entity>>(e.Data);

		for (auto& exp : explosion)
		{
			auto& player = m_Handle.Transform().Translation;
			auto& expl = exp.Transform().Translation;

			if (glm::distance(player, expl) < range)
			{
				// Bomb damage equals insta kill
				m_PlayerData.Health = 0;

				m_PlayerAnimator.Play("PlayerDead");

				auto body = static_cast<b2Body*>(m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody);
				body->SetLinearVelocity({ 0, 0 });

				Dispatch(GameEventType::PLAYER_GONE);
			}
		}
	}
	else if (e.Type == GameEventType::ENEMY_MOVED)
	{
		if (m_PlayerData.Invincibility != 0.0f)
			return;

		auto& entity = std::any_cast<Entity>(e.Data);

		if (glm::distance(entity.Transform().Translation, m_Handle.Transform().Translation) < 0.8f)
		{
			if (m_PlayerData.Health != 0) //TODO: invincibility when hit by a monster
			{
				m_PlayerData.Health--;
				m_PlayerData.Invincibility = 3.0f;

				if (m_PlayerData.Health == 0)
				{
					m_PlayerAnimator.Play("PlayerDead");

					auto body = static_cast<b2Body*>(m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody);
					body->SetLinearVelocity({ 0, 0 });

					Dispatch(GameEventType::PLAYER_GONE);
					return;
				}
				m_PlayerHit = true;
				Dispatch(GameEventType::PLAYER_HIT, m_PlayerData.Health);
			}
		}
	}
	else if (e.Type == GameEventType::BOMB_DEPLOY_UNSUCCESSFUL)
	{
		m_PlayerData.AvailableBombs++;
	}
	else if (e.Type == GameEventType::PLAYER_SUCCESS_EXIT)
	{
		auto body = static_cast<b2Body*>(m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody);
		body->SetLinearVelocity({ 0, 0 });
		m_Handle.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
		m_PlayerAnimator.SetColor(glm::vec4(0.0f));
		m_PlayerAnimator.Pause();
	}
	else if (e.Type == GameEventType::PLAYER_SPEED_UPGRADE)
	{
		m_PlayerData.Speed++;
		g_InGameData.SpeedUpgrade++;
	}
	else if (e.Type == GameEventType::PLAYER_POWER_UPGRADE)
	{
		m_PlayerData.Power++;
		g_InGameData.BombPowerUpgrade++;
	}
	else if (e.Type == GameEventType::PLAYER_BOMB_COUNT_UPGRADE)
	{
		if (g_InGameData.BombCountUpgrade < MAX_UPGRADE_COUNT_BOMBS)
		{
			m_PlayerData.AvailableBombs++;
			g_InGameData.BombCountUpgrade++;
		}
	}
	else if (e.Type == GameEventType::PLAYER_ADD_HEART)
	{
		if (m_PlayerData.Health < MAX_HEART)
		{
			m_PlayerData.Health++;
		}
	}
	else if (e.Type == GameEventType::PLAYER_SUCCESS_EXIT)
	{
		m_GameOver = true;
	}
}

// DEBUG
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
			Dispatch(GameEventType::PLAYER_DIR_UP);
			m_LastKey = kkey_up;
			break;
		}
		case kkey_down:
		{
			m_AnimationState = PlayerAnimationState::DOWN;
			m_PlayerAnimator.Play("DownAnimation");
			Dispatch(GameEventType::PLAYER_DIR_DOWN);

			m_LastKey = kkey_down;
			break;
		}
		case kkey_left:
		{
			m_AnimationState = PlayerAnimationState::LEFT;
			m_PlayerAnimator.Play("LeftAnimation");
			Dispatch(GameEventType::PLAYER_DIR_LEFT);

			m_LastKey = kkey_left;
			break;
		}
		case kkey_right:
		{
			m_AnimationState = PlayerAnimationState::RIGHT;
			m_PlayerAnimator.Play("LeftAnimation");
			Dispatch(GameEventType::PLAYER_DIR_RIGHT);

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
		Dispatch(GameEventType::PLAYER_IDLE);
		m_PlayerAnimator.Play("PlayerIdle");
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

	body->SetLinearVelocity({ m_PlayerData.Velocity.x, m_PlayerData.Velocity.y });


	// Dispatch to all observers that player has moved
	if(m_PlayerData.Velocity.x != 0 || m_PlayerData.Velocity.y != 0)
	{
		//HU_INFO("DAMN: {0}, {1}", m_PlayerData.Velocity.x, m_PlayerData.Velocity.y);
		Dispatch(GameEventType::PLAYER_MOVED, m_Handle.Transform().Translation);
	}
}

