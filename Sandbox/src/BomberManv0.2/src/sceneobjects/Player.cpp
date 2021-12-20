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

	m_Handle.GetComponent<SpriteRendererComponent>().SubTexture = SubTexture2D::CreateFromCoords(spriteSheet, { 4, 7 }, { 16, 16 });

	// PLAYER ANIMATIONS
	m_PlayerAnimator.SetTarget(m_Handle);
	AnimationBlock playerLeftAnimation, playerUpAnimation, playerDownAnimation, playerDeadAnimation;
	{
		playerLeftAnimation.Tag = "LeftAnimation";
		playerLeftAnimation.SpriteSize = { 16,16 };
		playerLeftAnimation.SpriteSheet = spriteSheet;
		playerLeftAnimation.Repeat = true;

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ {0, 7}, 100.0f });
		framesSpec.push_back({ {1, 7}, 100.0f });
		framesSpec.push_back({ {2, 7}, 100.0f });

		playerLeftAnimation.Load(framesSpec);
	}

	{
		playerUpAnimation.Tag = "UpAnimation";
		playerUpAnimation.SpriteSize = { 16,16 };
		playerUpAnimation.SpriteSheet = spriteSheet;
		playerUpAnimation.Repeat = true;

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ {6, 7}, 100.0f });
		framesSpec.push_back({ {7, 7}, 100.0f });
		framesSpec.push_back({ {0, 6}, 100.0f });

		playerUpAnimation.Load(framesSpec);
	}

	{
		playerDownAnimation.Tag = "DownAnimation";
		playerDownAnimation.SpriteSize = { 16,16 };
		playerDownAnimation.SpriteSheet = spriteSheet;
		playerDownAnimation.Repeat = true;

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ {3, 7}, 100.0f });
		framesSpec.push_back({ {4, 7}, 100.0f });
		framesSpec.push_back({ {5, 7}, 100.0f });
		framesSpec.push_back({ {4, 7}, 100.0f });

		playerDownAnimation.Load(framesSpec);
	}
	// Player dead animation
	{
		playerDeadAnimation.Tag = "PlayerDead";
		playerDeadAnimation.SpriteSize = { 16,16 };
		playerDeadAnimation.SpriteSheet = spriteSheet;
		//m_PlayerDead.Animation.emplace_back(FrameData{ nullptr,glm::vec4(0.0f), 300.0f });

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ { 1, 6 }, 300.0f });
		framesSpec.push_back({ { 2, 6 }, 300.0f });
		framesSpec.push_back({ { 3, 6 }, 300.0f });
		framesSpec.push_back({ { 4, 6 }, 300.0f });
		framesSpec.push_back({ { 5, 6 }, 300.0f });
		framesSpec.push_back({ { 6, 6 }, 300.0f });
		framesSpec.push_back({ { 7, 6 }, 300.0f });
		framesSpec.push_back({ {}, 300.0f, true });

		playerDeadAnimation.Load(framesSpec);
	}
	m_PlayerAnimator.Add(playerLeftAnimation);
	m_PlayerAnimator.Add(playerUpAnimation);
	m_PlayerAnimator.Add(playerDownAnimation);
	m_PlayerAnimator.Add(playerDeadAnimation);

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

void Player::OnUpdate(Timestep ts)
{
	m_PlayerAnimator.OnUpdate(ts);

	if (m_PlayerData.Health <= 0 && m_Alive)
	{
		m_Alive = false;

		m_PlayerAnimator.Switch("PlayerDead");
	}

	if (!m_Alive)
		return;

	m_PlayerData.Direction = { 0, 0 };

	if (Input::IsKeyPressed(Key::W))
	{
		m_PlayerData.Direction.y = 1;
		m_PlayerData.Rotated = false;
		m_Transform->Scale.x = glm::abs(m_Transform->Scale.x);

		if (m_PlayerAnimator.GetActiveAnimation().Tag != "UpAnimation")
			m_PlayerAnimator.Switch("UpAnimation");
	}
	if (Input::IsKeyPressed(Key::D))
	{
		timer1 += ts;
		if (timer1 >= pms)
		{
			timer1 = 0;
			//	AudioManager::Play(AudioType::PLAYER_HORIZONTAL_MOVEMENT);
		}

		m_PlayerData.Direction.x = 1;
		if (!m_PlayerData.Rotated)
		{
			m_PlayerData.Rotated = true;
			m_Transform->Scale.x *= (-1);
		}

		if (m_PlayerAnimator.GetActiveAnimation().Tag != "LeftAnimation")
			m_PlayerAnimator.Switch("LeftAnimation");
	}

	if (Input::IsKeyPressed(Key::S))
	{
		m_PlayerData.Direction.y = -1;
		m_PlayerData.Rotated = false;
		m_Transform->Scale.x = glm::abs(m_Handle.GetComponent<TransformComponent>().Scale.x);

		if (m_PlayerAnimator.GetActiveAnimation().Tag != "DownAnimation")
		{
			m_PlayerAnimator.Switch("DownAnimation");
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

		m_PlayerData.Direction.x = -1;
		m_PlayerData.Rotated = false;
		m_Transform->Scale.x = glm::abs(m_Handle.GetComponent<TransformComponent>().Scale.x);

		if (m_PlayerAnimator.GetActiveAnimation().Tag != "LeftAnimation")
			m_PlayerAnimator.Switch("LeftAnimation");
	}

	if (m_PlayerData.Direction.x == 0 && m_PlayerData.Direction.y == 0)
	{
		m_PlayerAnimator.Stop();
	}

	m_PlayerData.Velocity.x = m_PlayerData.Direction.x * m_PlayerData.Speed;
	m_PlayerData.Velocity.y = m_PlayerData.Direction.y * m_PlayerData.Speed;

	auto body = (b2Body*)m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody;
	body->SetLinearVelocity({ m_PlayerData.Velocity.x,m_PlayerData.Velocity.y });
}

void Player::OnKeyPressed(KeyPressedEvent& e)
{
}

void Player::OnKeyReleased(KeyReleasedEvent& e)
{
}