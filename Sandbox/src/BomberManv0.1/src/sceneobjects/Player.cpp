#include "Player.h"

#define NO_KEY_TYPED 0

#include "../scripts/PlayerControllerScript.h"
#include "../scripts/FrameAnimatorScript.h"

void Player::Init(Ref<Scene> scene, Ref<GameGrid> gamegrid)
{
	m_GameScene = scene;
	m_GameGrid = gamegrid;

	// Player Init
	m_PlayerTexture = Texture2D::Create("src/game-BomberFriends/assets/textures/player_animation/facingdown/1.png");
	m_PlayerEntity = m_GameScene->CreateEntityWithDrawOrder(4, "PlayerEntity");
	m_PlayerEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::PLAYER;
	m_PlayerEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = m_PlayerTexture;
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

	m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerControllerScript>();
	for (uint32_t y = 0; y < m_GameGrid->m_Columns; y++)
	{
		for (uint32_t x = 0; x < m_GameGrid->m_Rows; x++)
		{
			Cell& entity = m_GameGrid->m_GameGrid[y][x];
			if (entity.CellEntity.GetComponent<EntityTypeComponent>().Type == EntityType::TILE_PSP)
			{
				entity.CellEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				m_PlayerEntity.GetComponent<TransformComponent>().Translation = entity.CellEntity.GetComponent<TransformComponent>().Translation;
			}
		}
	}

	// Bomb Init
	m_BombAnimationSpriteSheet = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/ba_128x128.png");

	// Ticking
	#define BOMB_TICKING_ANIMATION_SIZE 11
	{
		m_BombTickingAnimation.Tag = "BombTicking";
		m_SpreadExplosionAnimation.SpriteSize = { 128,128 };
		m_SpreadExplosionAnimation.SpriteSheet = m_BombAnimationSpriteSheet;

		std::array<glm::vec2, BOMB_TICKING_ANIMATION_SIZE> positions;
		std::array<float, BOMB_TICKING_ANIMATION_SIZE> delays = { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, // Default delays 
															 50.0f, 30.0f/*, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f */ };
		// Layout is 9 x 2
		positions[0] = { 0,1 };
		positions[1] = { 1,1 };
		positions[2] = { 2,1 };
		positions[3] = { 3,1 };
		positions[4] = { 4,1 };
		positions[5] = { 5,1 };
		positions[6] = { 6,1 };
		positions[7] = { 7,1 };
		positions[8] = { 8,1 };
		positions[9] = { 0,0 };
		positions[10] = { 1,0 };

		m_BombTickingAnimation.Animation.reserve(BOMB_TICKING_ANIMATION_SIZE);
		for (size_t i = 0; i < BOMB_TICKING_ANIMATION_SIZE; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_BombAnimationSpriteSheet, positions[i], { 128, 128 });
			frameData.Delay = delays[i];

			m_BombTickingAnimation.Animation.emplace_back(frameData);
		}
	}
	// Explosion
	#define BOMB_EXPLOSION_ANIMATION_SIZE 7
	{
		m_SpreadExplosionAnimation.Tag = "BombExplosion";
		m_SpreadExplosionAnimation.SpriteSize = { 128,128 };
		m_SpreadExplosionAnimation.SpriteSheet = m_BombAnimationSpriteSheet;
		std::array<glm::vec2, BOMB_EXPLOSION_ANIMATION_SIZE> positions;
		std::array<float, BOMB_EXPLOSION_ANIMATION_SIZE> delays = { 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f, 20.0f };
		// Layout is 9 x 2
		positions[1] = { 2,0 };
		positions[2] = { 3,0 };
		positions[3] = { 4,0 };
		positions[4] = { 5,0 };
		positions[5] = { 6,0 };
		positions[6] = { 7,0 };
		positions[7] = { 8,0 };

		m_SpreadExplosionAnimation.Animation.reserve(BOMB_EXPLOSION_ANIMATION_SIZE);
		for (size_t i = 0; i < BOMB_EXPLOSION_ANIMATION_SIZE; i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(m_BombAnimationSpriteSheet, positions[i], { 128, 128 });
			frameData.Delay = delays[i];

			m_SpreadExplosionAnimation.Animation.emplace_back(frameData);
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
		b2Body* body =(b2Body*)m_PlacedBombs[i].GetBombEntity().GetComponent<Rigidbody2DComponent>().RuntimeBody;

		if (m_PlacedBombs[i].Tick(ts)) // If true bomb has exploded and vanished
		{
			for (auto& entity : m_PlacedBombs[i].GetSpreadEntities())
			{
				switch (entity.GetComponent<EntityTypeComponent>().Type)
				{
					case EntityType::TILE_BOX:
					{
						entity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
						m_GameScene->DestroyBody(entity);
						entity.RemoveComponent<Rigidbody2DComponent>();
						entity.RemoveComponent<BoxCollider2DComponent>();
						break;
					}
					case EntityType::BOMB_SPREAD_EXPLOSION:
					{
						entity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;

						entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
						entity.GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
						entity.RemoveComponent<AnimationComponent>();
						entity.RemoveComponent<NativeScriptComponent>();
						break;
					}
				}
			}
			m_GameScene->DestroyEntity(m_PlacedBombs[i].GetBombEntity());
			m_PlacedBombs.erase(m_PlacedBombs.begin() + i);
			m_PlayerStats.PlacedBombs--;
		}
	}
}

void Player::PlaceBomb()
{
	if (CanCreateAnother())
	{
		BombProperties props;

		props.BombEntity = m_GameScene->CreateEntityWithDrawOrder(3, "BombEntity");
		props.BombEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB;
		props.BombEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
		props.BombEntity.AddCustomComponent<AnimationComponent>();
		props.BombEntity.AddComponent<NativeScriptComponent>().Bind<FrameAnimator>();
		props.BombEntity.GetComponent<TransformComponent>().Translation.x = glm::round(m_PlayerEntity.GetComponent<TransformComponent>().Translation.x);
		props.BombEntity.GetComponent<TransformComponent>().Translation.y = glm::round(m_PlayerEntity.GetComponent<TransformComponent>().Translation.y);

		props.BombEntity.AddComponent<Rigidbody2DComponent>();
		props.BombEntity.AddComponent<BoxCollider2DComponent>().Trigger = true;
		m_GameScene->CreateBody(props.BombEntity);

		m_GameScene->ManuallyInstantiateScript(props.BombEntity);
		FrameAnimator* frameanimator = (FrameAnimator*)props.BombEntity.GetComponent<NativeScriptComponent>().Instance;

		frameanimator->Add(m_BombTickingAnimation);  
		frameanimator->Add(m_SpreadExplosionAnimation);  
		frameanimator->Switch("BombTicking"); // Switches and plays the animation

		props.Type = BombType::CLASSIC;
		props.Length = 2;
		props.Reach = 1;
		props.State = BombState::TICKING;

		bool found = false;
		for (size_t y = 1; y < m_GameGrid->m_Columns - 1; y++)
		{
			for (size_t x = 1; x < m_GameGrid->m_Rows - 1; x++)
			{
				Cell& cell = m_GameGrid->m_GameGrid[y][x];
				if (cell.CellEntity.GetComponent<TransformComponent>().Translation.x == props.BombEntity.GetComponent<TransformComponent>().Translation.x
					&& cell.CellEntity.GetComponent<TransformComponent>().Translation.y == props.BombEntity.GetComponent<TransformComponent>().Translation.y)
				{
					// Found the bomb
					props.SpreadEntities.reserve(4 * props.Reach);
					props.SpreadEntities.emplace_back(m_GameGrid->m_GameGrid[y][x - 1].CellEntity);
					props.SpreadEntities.emplace_back(m_GameGrid->m_GameGrid[y - 1][x].CellEntity);
					props.SpreadEntities.emplace_back(m_GameGrid->m_GameGrid[y][x + 1].CellEntity);
					props.SpreadEntities.emplace_back(m_GameGrid->m_GameGrid[y + 1][x].CellEntity);

					for (auto& entity : props.SpreadEntities)
					{
						switch (entity.GetComponent<EntityTypeComponent>().Type)
						{
							case EntityType::TILE_EMPTY:
							{
								entity.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
								entity.AddCustomComponent<AnimationComponent>();
								entity.AddCustomComponent<NativeScriptComponent>().Bind<FrameAnimator>();
								m_GameScene->ManuallyInstantiateScript(entity);
								FrameAnimator* frameAnimator = (FrameAnimator*)entity.GetComponent<NativeScriptComponent>().Instance;

								frameAnimator->Add(m_SpreadExplosionAnimation); // Second animation("BombExplosion")
							}
						}
					}
					found = true;
					break;
				}
			}
			if (found)
				break;
		}

		m_PlacedBombs.push_back(Bomb(props));
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
	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			if (lastKey != Key::B)
			{
				lastKey = Key::B;
				PlaceBomb();
			}
		}
	}
	return false;
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	lastKey = NO_KEY_TYPED;
	return false;
}