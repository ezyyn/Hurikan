#include "Player.h"

#include "../core/TextureLoader.h"
#include "../core/CustomComponents.h"
#include "../scripts/PlayerControllerScript.h"
#include "../scripts/PlayerAnimationComponent.h"

void Player::Init(Ref<Scene> scene, Ref<GameGrid> gamegrid)
{
	m_GameGrid = gamegrid;
	float time = 1;
	float frames = 9;

	// Cannot use #emplace_back because it would result in same copying as with #push_back ( FrameData struct would be copied anyway )  
	// TEMP 
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame1"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame2"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame3"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame4"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame5"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame6"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame7"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame8"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame9"), 100 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame10"), 50 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame11"), 30 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame12"), 20 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame13"), 20 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame14"), 20 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame15"), 20 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame16"), 20 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame17"), 20 });
	m_BombAnimation.push_back({TextureLoader::Get()->Load("Bomb-Frame18"), 20 });

	m_ExplosionAnimation.push_back({ TextureLoader::Get()->Load("Bomb-Frame14"), 20 });
	m_ExplosionAnimation.push_back({ TextureLoader::Get()->Load("Bomb-Frame15"), 20 });
	m_ExplosionAnimation.push_back({ TextureLoader::Get()->Load("Bomb-Frame16"), 20 });
	m_ExplosionAnimation.push_back({ TextureLoader::Get()->Load("Bomb-Frame17"), 20 });
	m_ExplosionAnimation.push_back({ TextureLoader::Get()->Load("Bomb-Frame18"), 20 });

	m_PlayerAnimations.push_back({ TextureLoader::Get()->Load("PlayerDown"), 100 });
	m_PlayerAnimations.push_back({ TextureLoader::Get()->Load("PlayerDown1"), 100 });
	m_PlayerAnimations.push_back({ TextureLoader::Get()->Load("PlayerDown2"), 100 });

	// Scene
	m_Scene = scene;
	m_PlayerEntity = m_Scene->CreateEntityWithDrawOrder(2,"PlayerEntity");
	m_PlayerEntity.AddCustomComponent<EntityTypeComponent>(EntityType::PLAYER);
	//m_PlayerEntity.AddComponent<FrameAnimation2DComponent>().Animation = m_PlayerAnimations;
	//m_PlayerEntity.GetComponent<FrameAnimation2DComponent>().Loop = true;
	m_PlayerEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }).Texture = TextureLoader::Get()->Load("PlayerDown");

	auto& pac = m_PlayerEntity.AddCustomComponent<PlayerAnimationComponent>(m_PlayerEntity);
	pac.SetAnimation(m_PlayerAnimations);

	auto& rb = m_PlayerEntity.AddComponent<Rigidbody2DComponent>();
	rb.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rb.Gravity = false;
	rb.FixedRotation = true;

	float scale = 0.96f;
	m_PlayerEntity.GetComponent<TransformComponent>().Scale.x = scale;
	m_PlayerEntity.GetComponent<TransformComponent>().Scale.y = scale;

	auto& start_position_entity = m_Scene->GetEntityByTag("PlayerStartingPositionEntity");
	start_position_entity.GetComponent<TagComponent>().Tag = "Empty";
	start_position_entity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
	m_PlayerEntity.GetComponent<TransformComponent>().Translation = start_position_entity.GetComponent<TransformComponent>().Translation;

	auto& cc2d = m_PlayerEntity.AddComponent<CircleCollider2DComponent>();
	cc2d.Radius = scale / 2.0f;
	cc2d.Friction = 0.0f;

	// EnTT does not support multiple components of a same type 
	//m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	m_PlayerEntity.AddCustomComponent<PlayerController>(m_PlayerEntity);
}

const int maxCount = 5;
bool B_pressed = false;
bool S_pressed = false;

void Player::OnUpdate(Timestep& ts)
{
	auto& controller = m_PlayerEntity.GetComponent<PlayerController>();
	auto& pac = m_PlayerEntity.GetComponent<PlayerAnimationComponent>();

	controller.OnUpdate(ts);
	pac.OnUpdate(ts);

	if (S_pressed)
	{

	}

	// Bomb Placement
	bool canCreateAnother = true;

	if (B_pressed)
	{
		B_pressed = false; // One pass per type

		if (count <= 0)
			canCreateAnother = false;

		auto& translation = m_PlayerEntity.GetComponent<TransformComponent>().Translation;
		for (auto bomb_entity : m_PlacedBombs)
		{
			if (glm::round(translation.x) == bomb_entity.GetEntity().GetComponent<TransformComponent>().Translation.x
				&& glm::round(translation.y) == bomb_entity.GetEntity().GetComponent<TransformComponent>().Translation.y)
			{
				// Cannot create another bomb entity if already exists on the position
				canCreateAnother = false;
			}
		}
		if (canCreateAnother)
		{
			auto& bomb_entity = m_Scene->CreateEntityWithDrawOrder(1, "BombEntity");
			
			bomb_entity.AddCustomComponent<EntityTypeComponent>(EntityType::BOMB);
			bomb_entity.AddComponent<SpriteRendererComponent>();
			bomb_entity.GetComponent<TransformComponent>().Translation = { glm::round(translation.x), glm::round(translation.y), translation.z };

			auto& pac = bomb_entity.AddCustomComponent<FrameAnimationComponent>(bomb_entity);
			pac.SetAnimation(m_BombAnimation);
			pac.Play();

			auto grid_entities = m_GameGrid->GetGridEntities().Blocks;
			std::vector<Entity> spread_explosion = {}; // 0 - Left, 1 - Right, 2 - Up, 3 - Down 
			for (int i = 17 + 1; i < grid_entities.size() - (17 + 1); i++)
			{
				auto block = grid_entities[i];
				
				auto left_block = grid_entities[i - 1];
				auto right_block = grid_entities[i + 1];
				auto up_block = grid_entities[i - 17];
				auto down_block = grid_entities[i + 17];
				
				// Center
				if (block.GetComponent<TransformComponent>().Translation.x == bomb_entity.GetComponent<TransformComponent>().Translation.x &&
					block.GetComponent<TransformComponent>().Translation.y == bomb_entity.GetComponent<TransformComponent>().Translation.y)
				{
					spread_explosion.push_back(left_block);
					spread_explosion.push_back(right_block);
					spread_explosion.push_back(up_block);
					spread_explosion.push_back(down_block);
				}
			}

			Bomb bomb(m_Scene, bomb_entity, spread_explosion, m_ExplosionAnimation);
			m_PlacedBombs.push_back(bomb);
			count--;
		}
	}
	for (size_t i = 0; i < m_PlacedBombs.size(); i++)
	{
		bool destroyable = m_PlacedBombs[i].Tick(ts);

		if (destroyable)
		{
			m_PlacedBombs.erase(m_PlacedBombs.begin() + i);
			count++;
		}
	}

#if 0
	for (size_t i = 0; i < m_PlacedBombs.size(); i++)
	{
		FrameAnimation2DScript* animation_script = (FrameAnimation2DScript*)(m_PlacedBombs[i].GetComponent<NativeScriptComponent>().Instance);

		static bool createOncePerBomb = true; // TODO: fix this garbage, 
		if (animation_script->GetAnimationIndex() == 13 && createOncePerBomb)
		{
			createOncePerBomb = false;
			for (size_t j = 0; j < 4; j++)
			{
				Entity& explosion = m_Scene->CreateEntity("BombEntity-Explosion" + std::to_string(i) + std::to_string(j));
				// Speading the explosion

				explosion.AddComponent<SpriteRendererComponent>();
				explosion.AddComponent<FrameAnimation2DComponent>().Animation = m_ExplosionAnimation;
				explosion.GetComponent<TransformComponent>().Translation = m_PlacedBombs[i].GetComponent<TransformComponent>().Translation;
				switch (j)
				{
					case 0:
						explosion.GetComponent<TransformComponent>().Translation.x--;
						break;
					case 1:
						explosion.GetComponent<TransformComponent>().Translation.x++;
						break;
					case 2:
						explosion.GetComponent<TransformComponent>().Translation.y--;
						break;
					case 3:
						explosion.GetComponent<TransformComponent>().Translation.y++;
						break;
				}

				auto& nsc = explosion.AddComponent<NativeScriptComponent>();
				nsc.Bind<FrameAnimation2DScript>();
				m_Scene->ManuallyInstantiateScript(explosion);
				FrameAnimation2DScript* instance = (FrameAnimation2DScript*)nsc.Instance;
				instance->Play();
			}
		}
		

		if (animation_script != nullptr && !animation_script->IsAnimationPlaying())
		{
			for (size_t j = 0; j < 4; j++)
			{
				Entity& explosion = m_Scene->GetEntityByTag("BombEntity-Explosion" + std::to_string(i) + std::to_string(j));
				m_Scene->DestroyEntity(explosion);
			}

			createOncePerBomb = true;
			count++;
			m_Scene->DestroyEntity(m_PlacedBombs[i]);
			m_PlacedBombs.erase(m_PlacedBombs.begin() + i);
		}
	}
#endif
}

void Player::OnUpdate(Timestep ts)
{

}

bool d = true;

bool Player::OnKeyPressed(KeyPressedEvent& e)
{
	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			if (d)
			{
				d = false;
				B_pressed = true;
				S_pressed = true;
				break;
			}
		}
	}
	return false;
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			d = true;
			B_pressed = false;
			S_pressed = false;
			break;
		}
	}
	return false;
}

