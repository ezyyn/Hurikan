#include "Bomb.h"

#include "../core/GameComponents.h"

void Bomb::Init(BombProperties props, Player* player)
{
	// Initializing global variables
	g_Player = player;
	g_GameScene = g_Player->g_GameScene;

	// Initializing members
	m_Properties = props;

	// Creating bomb entity with ECS
	m_Entity = g_GameScene->CreateEntityWithDrawOrder(3, "BombEntity");

	// Setting up newly created entity to be placed on the rounded position of the player
	m_Entity.GetComponent<TransformComponent>().Translation.x = glm::round(g_Player->m_PlayerEntity.GetComponent<TransformComponent>().Translation.x);
	m_Entity.GetComponent<TransformComponent>().Translation.y = glm::round(g_Player->m_PlayerEntity.GetComponent<TransformComponent>().Translation.y);

	// TODO: FIX OVERLAPPING (optional)
	
	player->m_GameGrid->ForEach_1([&m_Entity = m_Entity, &m_GridEntityPlaceHolders = m_GridEntityPlaceHolders](Entity gridEntity) 
	{
		if (m_Entity.GetComponent<TransformComponent>().Translation == gridEntity.GetComponent<TransformComponent>().Translation)
		{
			gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB;
			m_GridEntityPlaceHolders.push_back(gridEntity);
			return true;
		}
		return false;
	});
	

	// Setting up category to which entity belongs
	m_Entity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB;

	// Setting up rendering
	m_Entity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));

	// Setting up physics
	m_Entity.AddComponent<Rigidbody2DComponent>();
	// Trigger is true so player can more on the bomb
	// If player steps out of the bomb, ergo not colliding with it,player will no longer can step on the bomb (see #CollisionDetector.h)
	m_Entity.AddComponent<BoxCollider2DComponent>().Trigger = true;
	// Manually creating b2Body in Scene.h due to runtime // TODO: do something with it 
	g_GameScene->CreateBody(m_Entity);
	// Setting up animations for the entity
	auto& frameanimator = m_Entity.AddCustomComponent<FrameAnimator>(m_Entity);
	frameanimator.Add(g_Player->m_BombTickingAnimation);
	frameanimator.Switch("BombTicking"); 
	m_Properties.State = BombState::TICKING;
}

bool Bomb::WingInitialization(uint32_t y, uint32_t distance, bool condition, float rotationZ, std::vector<Entity>& wing)
{
	if (condition)
	{
		// Grid Entity
		Entity gridEntity = g_Player->m_GameGrid->m_GameGrid[y][distance];

		// Getting GridEntity coordinates so it can create new entity 
		// And eventually adding it to the wing #vector
		auto& grid_translation = gridEntity.GetComponent<TransformComponent>().Translation;

		// Checking the of the GridEntity
		switch (gridEntity.GetComponent<EntityTypeComponent>().Type)
		{
			// Stops expanding due to collision with a wall
			case EntityType::TILE_WALL: return true;

			// Breakable
			// Stops expanding due to collision with a box
			case EntityType::TILE_BOX:
			{
				// Adding it to another separated #m_Destroyed vector
				// Every GridEntity in this vector will be destroyed ( Not the ECS entity )
				m_Destroyed.push_back(gridEntity);

				// When single destoyable wall ( box ) is going to be destroyed by two bomb at once,
				// the second bomb will not add spread explosion entity because, it is not be
				// yet destroyed( marked as TILE_EMPTY ) 
				// This line is necessary and prevents this bug
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_TO_BE_EMPTY;
				return true;
			}
			// Expands the explosion
			case EntityType::TILE_TO_BE_EMPTY:
			case EntityType::TILE_EMPTY:
			{
				Entity spread = g_GameScene->CreateEntityWithDrawOrder(3, "WingEntity");
				// TODO: overlapping
				/*gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
				m_GridEntityPlaceHolders.push_back(gridEntity);*/

				spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
				spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
			
				spread.GetComponent<TransformComponent>().Translation = grid_translation;
				spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(rotationZ);
				wing.push_back(spread);
				return false;
			}
			case EntityType::BOMB: return true;
		}
		return false;
	}
	return true;
}

void Bomb::AddAnimations(std::vector<Entity> wing)
{
	for (size_t i = 0; i < wing.size(); i++)
	{
		auto& fa = wing[i].AddCustomComponent<FrameAnimator>(wing[i]);
		if (wing.size() == m_Properties.Reach && i == wing.size() - 1)
		{
			fa.Add(g_Player->m_BombSpreadEndWingAnimation);
			fa.Switch("BombEndWingExplosion");
			break;
		}
		fa.Add(g_Player->m_BombSpreadWingAnimation);
		fa.Switch("BombWingExplosion");
	}
}

void Bomb::Deploy()
{
	// Iterating through the whole GameGrid
	// Except borders to prevent array exception
	// Basic Game Mechanic
	for (uint32_t y = 1; y < g_Player->m_GameGrid->GetColumns() - 1; y++)
	{
		for (uint32_t x = 1; x < g_Player->m_GameGrid->GetRows() - 1; x++)
		{
			// Searhing for the same coordinates as chosen GridEntity so 

			Entity gridentity = g_Player->m_GameGrid->m_GameGrid[y][x];
			auto& trans = gridentity.GetComponent<TransformComponent>().Translation;
			// Condition below is searching for the same entity translation coordinates as the bomb.
			// So it can properly know where spread wings should be
			if (trans == this->translation()) 
			{ 
				for (uint32_t j = 0; j < m_Properties.Reach; j++)
				{
					int index = x - (j + 1);
					if (WingInitialization(y, index, index >= 1, 90.0f, m_LeftWing))
						break;
				}

				AddAnimations(m_LeftWing);

				for (uint32_t j = 0; j < m_Properties.Reach; j++)
				{
					int index = y - (j + 1);
					if (WingInitialization(index, x, index >= 1, 0.0f, m_UpWing)) 
						break;
				}
				AddAnimations(m_UpWing);

				for (uint32_t j = 0; j < m_Properties.Reach; j++)
				{
					int index = x + (j + 1);
					if (WingInitialization(y, index, index < g_Player->m_GameGrid->GetColumns(), 270.0f, m_RightWing)) 
						break;
				}
				AddAnimations(m_RightWing);

				for (uint32_t j = 0; j < m_Properties.Reach; j++)
				{
					int index = y + (j + 1);
					if (WingInitialization(index, x, index < g_Player->m_GameGrid->GetRows(), 180.0f, m_DownWing)) 
						break;
				}
				AddAnimations(m_DownWing);
			}
		}
	}


	Entity center = g_GameScene->CreateEntityWithDrawOrder(3, "CenterWingEntity");
	center.GetComponent<TransformComponent>().Translation = this->translation();

	center.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
	center.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;

	auto& fa = center.AddCustomComponent<FrameAnimator>(center);
	fa.Add(g_Player->m_BombSpreadCenterAnimation);
	fa.Switch("BombCenterExplosion");

	m_Properties.SpreadEntities.reserve(5);
	m_Properties.SpreadEntities.emplace_back(std::vector<Entity>{ center });
	m_Properties.SpreadEntities.emplace_back(m_LeftWing);
	m_Properties.SpreadEntities.emplace_back(m_UpWing);
	m_Properties.SpreadEntities.emplace_back(m_RightWing);
	m_Properties.SpreadEntities.emplace_back(m_DownWing);
}

bool Bomb::Tick(Timestep ts)
{
	if (m_Properties.State == BombState::TICKING)
	{
		m_Properties.Time += ts;

		auto& frameanimator = m_Entity.GetComponent<FrameAnimator>();
		frameanimator.OnUpdate(ts);
	}

	if (m_Properties.Time >= m_Properties.ExplosionTime && m_Properties.State == BombState::TICKING || m_ChainExplosion) // Explosion
	{
		m_Properties.State = BombState::EXPLOSION;

		// Check for chain reaction
		for (auto& other : g_Player->m_PlacedBombs)
		{
			for (auto& wings : m_Properties.SpreadEntities) // Spread // BASIC BOMB MECHANIC IMPLEMENTED ( ??? ) ROOM FOR TESTING
			{
				for (auto& entity : wings)
				{
					if (other.m_Properties.State != BombState::EXPLOSION &&
						other.GetBombEntity().GetComponent<TransformComponent>().Translation == entity.GetComponent<TransformComponent>().Translation)
					{
						other.Explode();
					}
				}
			}
		}

		m_Entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		// Setting alpha value to 1 
		for (auto& wings : m_Properties.SpreadEntities) 
		{
			for (auto& entity : wings)
			{
				entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}
		}
		// Destroying boxes in the way
		// Playing vanishing wall break animation
		for (auto& tobedestroyed : m_Destroyed) 
		{
			tobedestroyed.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");
		}
	}

	if (m_Properties.State == BombState::EXPLOSION)
	{
		for (auto& wings : m_Properties.SpreadEntities) // Explosion
		{
			for (auto& entity : wings)
			{
				auto& frameanimator = entity.GetComponent<FrameAnimator>();
				frameanimator.OnUpdate(ts);

				if (!frameanimator.IsAnyPlaying())
				{
					// Animation finally complete and can be safely removed
					DestroyItSelf();
					return true; 
				}
			}
		}
	}
	return false;
}

void Bomb::DestroyItSelf()
{
	// Destroys all entities related to bomb
	g_GameScene->DestroyEntity(m_Entity);

	for (auto& entity : m_GridEntityPlaceHolders)
	{
		g_Player->m_GameGrid->DestroyGridEntity(entity);
	}
	for (auto& wings : m_Properties.SpreadEntities) 
	{
		for (auto& entity : wings)
		{
			g_GameScene->DestroyEntity(entity);
		}
	}

	for (auto& gridentity : m_GridEntityPlaceHolders)
	{
		gridentity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
	}
	m_GridEntityPlaceHolders.clear();
	m_GridEntityPlaceHolders.shrink_to_fit();

	// Freeing up the memory
	g_Player = nullptr;
	g_GameScene = nullptr;
	m_GridEntityPlaceHolders.clear();
	m_GridEntityPlaceHolders.shrink_to_fit();
	m_Properties.SpreadEntities.clear();
	m_Properties.SpreadEntities.shrink_to_fit();
}

void Bomb::Explode()
{
	m_ChainExplosion = true;
}

