#include "Player.h"

#define NO_KEY_TYPED 0

#include "../scripts/PlayerScript.h"
#include "../scripts/FrameAnimatorScript.h"

void Player::Init(Scene* scene, GameGrid* gamegrid)
{
	m_GameScene = scene;
	m_GameGrid = gamegrid;

	m_BombAnimationSpriteSheet = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/ba_128x128.png");
	Ref<Texture2D> SpriteSheet = Texture2D::Create("src/game-BomberFriends/assets/textures/player_animation/Chr_00_0.png");
	Ref<Texture2D> SpriteSheet1 = Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_16x16.png");

	// Player Init
	m_PlayerTexture = Texture2D::Create("src/game-BomberFriends/assets/textures/player_animation/facingdown/1.png");
	m_PlayerEntity = m_GameScene->CreateEntityWithDrawOrder(4, "PlayerEntity");
	m_PlayerEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::PLAYER;
	m_PlayerEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
	auto& frameanimator = (FrameAnimator&)m_PlayerEntity.AddCustomComponent<FrameAnimator>(m_PlayerEntity);
	m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerScript>();
	auto* playerscript = m_PlayerEntity.GetNativeScript<PlayerScript>();

	m_PlayerEntity.GetComponent<SpriteRendererComponent>().SubTexture = SubTexture2D::CreateFromCoords(SpriteSheet, { 4, 7 },{ 16, 16 });


	// NEW ANIMATIONS
	{
		m_PlayerLeftAnimation.Tag = "LeftAnimation";
		m_PlayerLeftAnimation.SpriteSize = { 16,16 };
		m_PlayerLeftAnimation.SpriteSheet = SpriteSheet;
		m_PlayerLeftAnimation.Repeat = true;
		std::array<glm::vec2, 3> positions;
		std::array<float, 3> delays = { 100.0f, 100.0f,100.0f };

		positions[0] = { 0,7 };
		positions[1] = { 1,7 };
		positions[2] = { 2,7 };

		m_PlayerLeftAnimation.Animation.reserve(3);
		for (size_t i = 0; i < 3; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_PlayerLeftAnimation.SpriteSheet, positions[i], m_PlayerLeftAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_PlayerLeftAnimation.Animation.emplace_back(frameData);
		}
	}

	{
		m_PlayerUpAnimation.Tag = "UpAnimation";
		m_PlayerUpAnimation.SpriteSize = { 16,16 };
		m_PlayerUpAnimation.SpriteSheet = SpriteSheet;
		m_PlayerUpAnimation.Repeat = true;
		std::array<glm::vec2, 3> positions;
		std::array<float, 3> delays = { 100.0f, 100.0f,100.0f };

		positions[0] = { 6,7 };
		positions[1] = { 7,7 };
		positions[2] = { 0,6 };

		m_PlayerUpAnimation.Animation.reserve(3);
		for (size_t i = 0; i < 3; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_PlayerUpAnimation.SpriteSheet, positions[i], m_PlayerLeftAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_PlayerUpAnimation.Animation.emplace_back(frameData);
		}
	}

	{
		m_PlayerDownAnimation.Tag = "DownAnimation";
		m_PlayerDownAnimation.SpriteSize = { 16,16 };
		m_PlayerDownAnimation.SpriteSheet = SpriteSheet;
		m_PlayerDownAnimation.Repeat = true;
		std::array<glm::vec2, 4> positions;
		std::array<float, 4> delays = { 100.0f, 100.0f, 100.0f, 100.0f };

		positions[0] = { 3,7 };
		positions[1] = { 4,7 };
		positions[2] = { 5,7 };
		positions[3] = { 4,7 };

		m_PlayerDownAnimation.Animation.reserve(positions.size());
		for (size_t i = 0; i < positions.size(); i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_PlayerDownAnimation.SpriteSheet, positions[i], m_PlayerLeftAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_PlayerDownAnimation.Animation.emplace_back(frameData);
		}
	}

	frameanimator.Add(m_PlayerLeftAnimation);
	frameanimator.Add(m_PlayerUpAnimation);
	frameanimator.Add(m_PlayerDownAnimation);

	/////////////////////////////////////////////////////////////////////
	////////////////////     Player Physics     ////////////////////////
	///////////////////////////////////////////////////////////////////

	auto& rb2d = m_PlayerEntity.AddComponent<Rigidbody2DComponent>();
	rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rb2d.Gravity = false;
	rb2d.FixedRotation = true;

	float scale = 0.96f;
	auto& scale_cmp = m_PlayerEntity.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

	auto& cc2d = m_PlayerEntity.AddComponent<CircleCollider2DComponent>();
	cc2d.Radius = scale / 2.0f;
	cc2d.Friction = 0.0f;

	m_GameGrid->ForEach([&m_PlayerEntity = m_PlayerEntity](uint32_t x, uint32_t y, Entity entity) 
		{
			if (entity.GetComponent<EntityTypeComponent>().Type == EntityType::TILE_PSP)
			{
				m_PlayerEntity.GetComponent<TransformComponent>().Translation = entity.GetComponent<TransformComponent>().Translation;
				entity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				return true;
			}
			return false;
		}
	);

	// Bomb Init

	// Ticking
	#define BOMB_TICKING_ANIMATION_SIZE 3
	{
		m_BombTickingAnimation.Tag = "BombTicking";
		m_BombTickingAnimation.SpriteSize = { 16,16 };
		m_BombTickingAnimation.SpriteSheet = SpriteSheet1;
		m_BombTickingAnimation.Repeat = true;
		std::array<glm::vec2, BOMB_TICKING_ANIMATION_SIZE> positions; // x000ms  <- must be divided by 9 frames 2
		std::array<float, BOMB_TICKING_ANIMATION_SIZE> delays = { 300.0f, 300.0f, 300.0f };
		// Layout is 9 x 2
		positions[0] = { 0,7 };
		positions[1] = { 1,7 };
		positions[2] = { 2,7 };

		m_BombTickingAnimation.Animation.reserve(BOMB_TICKING_ANIMATION_SIZE);
		for (size_t i = 0; i < BOMB_TICKING_ANIMATION_SIZE; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_BombTickingAnimation.SpriteSheet, positions[i], m_BombTickingAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_BombTickingAnimation.Animation.emplace_back(frameData);
		}
	}

	// NEW EXPLOSION
#define BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE 7
	{
		m_BombSpreadCenterAnimation.Tag = "BombCenterExplosion";
		m_BombSpreadCenterAnimation.SpriteSize = { 16,16 };
		m_BombSpreadCenterAnimation.SpriteSheet = SpriteSheet1;
		std::array<glm::vec2, BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE> positions;
		std::array<float, BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE> delays = { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f };
		// Layout is 9 x 2
		positions[0] = { 0, 1 };
		positions[1] = { 2, 1 };
		positions[2] = { 4, 1 };
		positions[3] = { 6, 1 };
		positions[4] = { 4, 1 };
		positions[5] = { 2, 1 };
		positions[6] = { 0, 1 };

		m_BombSpreadCenterAnimation.Animation.reserve(BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE);
		for (size_t i = 0; i < BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_BombSpreadCenterAnimation.SpriteSheet, positions[i], m_BombSpreadCenterAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_BombSpreadCenterAnimation.Animation.emplace_back(frameData);
		}
	}

	{
		m_BombSpreadWingAnimation.Tag = "BombWingExplosion";
		m_BombSpreadWingAnimation.SpriteSize = { 16,16 };
		m_BombSpreadWingAnimation.SpriteSheet = SpriteSheet1;
		std::array<glm::vec2, BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE> positions;
		std::array<float, BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE> delays = { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f };
		// Layout is 9 x 2
		positions[0] = { 0, 3 };
		positions[1] = { 2, 3 };
		positions[2] = { 4, 3 };
		positions[3] = { 6, 3 };
		positions[4] = { 4, 3 };
		positions[5] = { 2, 3 };
		positions[6] = { 0, 3 };

		m_BombSpreadWingAnimation.Animation.reserve(BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE);
		for (size_t i = 0; i < BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_BombSpreadWingAnimation.SpriteSheet, positions[i], m_BombSpreadWingAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_BombSpreadWingAnimation.Animation.emplace_back(frameData);
		}
	}

	{
		m_BombSpreadEndWingAnimation.Tag = "BombEndWingExplosion";
		m_BombSpreadEndWingAnimation.SpriteSize = { 16,16 };
		m_BombSpreadEndWingAnimation.SpriteSheet = SpriteSheet1;
		std::array<glm::vec2, BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE> positions;
		std::array<float, BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE> delays = { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f };
		// Layout is 9 x 2
		positions[0] = { 0, 5 };
		positions[1] = { 2, 5 };
		positions[2] = { 4, 5 };
		positions[3] = { 6, 5 };
		positions[4] = { 4, 5 };
		positions[5] = { 2, 5 };
		positions[6] = { 0, 5 };

		m_BombSpreadEndWingAnimation.Animation.reserve(BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE);
		for (size_t i = 0; i < BOMB_EXPLOSION_CENTER_SPREAD_ANIMATION_SIZE; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_BombSpreadEndWingAnimation.SpriteSheet, positions[i], m_BombSpreadEndWingAnimation.SpriteSize);
			frameData.Delay = delays[i];

			m_BombSpreadEndWingAnimation.Animation.emplace_back(frameData);
		}
	}
}

// Events
KeyCode key_typed = NO_KEY_TYPED;
bool key_hold = false;

void Player::OnUpdate(Timestep ts)
{
	for (size_t i = 0; i < m_PlacedBombs.size(); i++)
	{
		if (m_PlacedBombs[i].Tick(ts)) // If true bomb has exploded and vanished
		{
			for (auto& entity : m_PlacedBombs[i].m_Destroyed)
			{
				m_DestroyedBoxes.push_back(entity);
			}
			m_PlacedBombs.erase(m_PlacedBombs.begin() + i);
			m_PlayerStats.PlacedBombs--;
		}
	}

	for (size_t i = 0; i < m_DestroyedBoxes.size(); i++)
	{
		auto& fa = m_DestroyedBoxes[i].GetComponent<FrameAnimator>();
		//fa.OnUpdate(ts);

		if (!fa.IsAnyPlaying())
		{
			m_GameGrid->DestroyGridEntity(m_DestroyedBoxes[i]);
			m_DestroyedBoxes.erase(m_DestroyedBoxes.begin() + i);
		}
	}
}

void Player::PlaceBomb()
{
	if (CanCreateAnother())
	{
		// TODO: move this to Bomb.h
		BombProperties props;

		props.Type = BombType::CLASSIC;
		props.ExplosionTime = 1.0f;
		props.Reach = 1;
		props.State = BombState::TICKING;

		Bomb bomb;
		bomb.Init(props, this);
		bomb.Deploy();
		m_PlacedBombs.push_back(bomb);
		m_PlayerStats.PlacedBombs++;
	}
}

bool Player::CanCreateAnother()
{
	if (m_PlayerStats.PlacedBombs >= m_PlayerStats.MaxBombCount)
		return false;

	auto& translation = m_PlayerEntity.GetComponent<TransformComponent>().Translation;
	for (auto bomb_entity : m_PlacedBombs)
	{
		if (glm::round(translation.x) == bomb_entity.GetBombEntity().GetComponent<TransformComponent>().Translation.x
			&& glm::round(translation.y) == bomb_entity.GetBombEntity().GetComponent<TransformComponent>().Translation.y)
		{
			// Cannot create another bomb entity if already exists on the position
			return false;
		}
	}

	return true;
}

KeyCode lastKey = NO_KEY_TYPED;
bool Player::OnKeyPressed(KeyPressedEvent& e)
{
	// TODO: find better way to check this
	static bool scaleNegate = true;

	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			if (lastKey != Key::B)
				PlaceBomb();
			break;
		}
	}

	lastKey = e.GetKeyCode();
	return false;
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	lastKey = NO_KEY_TYPED;
	return false;
}