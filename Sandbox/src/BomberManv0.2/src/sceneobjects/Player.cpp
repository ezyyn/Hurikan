#include "Player.h"

#include <box2d/b2_body.h>

#include "../sceneobjects/Game.h"

extern Scene* g_GameScene;

void Player::Init()
{
	m_Handle = g_GameScene->CreateEntityWithDrawOrder(3, "Player");
	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
//	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::PLAYER;
	// Player animations 
	Ref<Texture2D> bombAnimationSpriteSheet = Texture2D::Create("assets/textures/bomb_animation/ba_128x128.png");
	Ref<Texture2D> spriteSheet = Texture2D::Create("assets/textures/player_animation/Chr_00_0.png");
	Ref<Texture2D> spriteSheet1 = Texture2D::Create("assets/textures/tileset_16x16.png");

	m_IdleAnimation = SubTexture2D::CreateFromCoords(spriteSheet, { 4, 7 }, { 16, 16 });
	m_Handle.GetComponent<SpriteRendererComponent>().SubTexture = m_IdleAnimation;
	// PLAYER ANIMATIONS
	m_PlayerAnimator.SetTarget(m_Handle);

	m_PlayerAnimator.Add(ResourceManager::RequestAnimation("PlayerLeftAnimation"));
	m_PlayerAnimator.Add(ResourceManager::RequestAnimation("PlayerUpAnimation"));
	m_PlayerAnimator.Add(ResourceManager::RequestAnimation("PlayerDownAnimation"));
	m_PlayerAnimator.Add(ResourceManager::RequestAnimation("PlayerDeadAnimation"));

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

	m_Transform = &m_Handle.Transform();
}

float pms;
float timer1 = pms;
float timer2 = 0;

KeyCode m_LastKey;
KeyCode m_PressedKey;

void Player::OnUpdate(Timestep ts)
{
	m_PlayerAnimator.OnUpdate(ts);

	if (m_PlayerData.Health <= 0 && m_Alive)
	{
		m_AnimationState = PlayerAnimationState::DEAD;
		m_Alive = false;
		m_PlayerAnimator.Play("PlayerDead");
	}

	if (!m_Alive)
		return;

	m_IsMoving = m_LastPosition != m_Transform->Translation;

	if (m_IsMoving)
	{
		m_LastPosition = m_Transform->Translation;
	}

	m_PlayerData.Direction = { 0, 0 };

	// Movement Movement Movement Movement Movement Movement Movement Movement Movement
	// Movement Movement Movement Movement Movement Movement Movement Movement Movement
	// Movement Movement Movement Movement Movement Movement Movement Movement Movement 
	if (Input::IsKeyPressed(Key::W))
	{
		m_PlayerData.Direction.y = 1;
		m_PlayerData.Rotated = false;
		m_Transform->Scale.x = glm::abs(m_Transform->Scale.x);

		m_PressedKey = Key::W;
	}

	if (Input::IsKeyPressed(Key::D))
	{
		m_PlayerData.Direction.x = 1;
		if (!m_PlayerData.Rotated)
		{
			m_PlayerData.Rotated = true;
			m_Transform->Scale.x *= (-1);
		}
		m_PressedKey = Key::D;
	} 

	if (Input::IsKeyPressed(Key::S))
	{
		m_PlayerData.Direction.y = -1;
		m_PlayerData.Rotated = false;
		m_Transform->Scale.x = glm::abs(m_Handle.GetComponent<TransformComponent>().Scale.x);
		m_PressedKey = Key::S;
	} 
	
	if (Input::IsKeyPressed(Key::A))
	{
		m_PlayerData.Direction.x = -1;
		m_PlayerData.Rotated = false;
		m_Transform->Scale.x = glm::abs(m_Handle.GetComponent<TransformComponent>().Scale.x);
		m_PressedKey = Key::A;
	}

	if (m_PressedKey != m_LastKey)
		switch (m_PressedKey)
		{
			case Key::W:
			{
				m_AnimationState = PlayerAnimationState::UP;
				m_PlayerAnimator.Play("UpAnimation");
				m_LastKey = Key::W;
				break;
			}
			case Key::S:
			{
				m_AnimationState = PlayerAnimationState::DOWN;
				m_PlayerAnimator.Play("DownAnimation");
				m_LastKey = Key::S;
				break;
			}
			case Key::A:
			{
				m_AnimationState = PlayerAnimationState::LEFT;
				m_PlayerAnimator.Play("LeftAnimation");
				m_LastKey = Key::A;
				break;
			}
			case Key::D:
			{
				m_AnimationState = PlayerAnimationState::RIGHT;
				m_PlayerAnimator.Play("LeftAnimation");
				m_LastKey = Key::D;
				break;
			}
			default:
			{
				//m_PlayerData.Velocity.x = 0;
				//m_PlayerData.Velocity.y = 0;
				break;
			}
		}

	if (m_PlayerData.Direction.x == 0 && m_PlayerData.Direction.y == 0)
	{
		m_AnimationState = PlayerAnimationState::IDLE;
		m_Handle.GetComponent<SpriteRendererComponent>().SubTexture = m_IdleAnimation; // <--- TODO: replace by an animation
		m_PlayerAnimator.Pause();
		m_LastKey = 0;
	}

	m_PlayerData.Velocity.x = m_PlayerData.Direction.x * m_PlayerData.Speed;
	m_PlayerData.Velocity.y = m_PlayerData.Direction.y * m_PlayerData.Speed;

	auto body = (b2Body*)m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody;

	if (glm::abs(m_PlayerData.Velocity.x) == 1 && glm::abs(m_PlayerData.Velocity.y) == 1)
	{
		switch (m_LastKey)
		{
		case Key::W:
			m_PlayerData.Velocity.x = 0;
			break;
		case Key::S:
			m_PlayerData.Velocity.x = 0;
			break;
		case Key::A:
			m_PlayerData.Velocity.y = 0;
			break;
		case Key::D:
			m_PlayerData.Velocity.y = 0;
			break;
		default:
		{
			m_PlayerData.Velocity.x = 0;
			m_PlayerData.Velocity.y = 0;
			break;
		}
		}
	}
	body->SetLinearVelocity({ m_PlayerData.Velocity.x, m_PlayerData.Velocity.y });
}

void Player::OnKeyPressed(KeyPressedEvent& e)
{
	switch (e.GetKeyCode())
	{
	case Key::W:
		break;
	case Key::S:
		break;
	case Key::A:
		break;
	case Key::D:
		break;
	default:
		break;
	}

	//m_LastKey = e.GetKeyCode();
}

void Player::OnKeyReleased(KeyReleasedEvent& e)
{
}