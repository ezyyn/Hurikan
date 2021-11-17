#include "Bomb.h"

#include "../core/GameComponents.h"

void Bomb::Init(BombProperties props, Player* player)
{
	// Initialiting global variables
	m_Player = player;
	m_GameScene = m_Player->m_GameScene;

	// Initialiting members
	m_Properties = props;

	// Creating bomb entity with ECS
	m_Entity = m_GameScene->CreateEntityWithDrawOrder(3, "BombEntity");

	// Setting up newly created entity to be placed on the rounded position of the player
	m_Entity.GetComponent<TransformComponent>().Translation.x = glm::round(m_Player->m_PlayerEntity.GetComponent<TransformComponent>().Translation.x);
	m_Entity.GetComponent<TransformComponent>().Translation.y = glm::round(m_Player->m_PlayerEntity.GetComponent<TransformComponent>().Translation.y);

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
	m_GameScene->CreateBody(m_Entity);
	// Setting up animations for the entity
	auto& frameanimator = m_Entity.AddCustomComponent<FrameAnimator>(m_Entity);
	frameanimator.Add(m_Player->m_BombTickingAnimation);
	frameanimator.Switch("BombTicking"); 
	m_Properties.State = BombState::TICKING;
}

void Bomb::initSpread(Entity spread)
{
	spread.GetComponent<TransformComponent>().Translation = this->translation();

	spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
	spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
}

void Bomb::Deploy()
{
	// Iterating through the whole GameGrid
	// Except borders to prevent array exception
	// Basic Game Mechanic
	for (uint32_t y = 1; y < m_Player->m_GameGrid->GetColumns() - 1; y++)
	{
		for (uint32_t x = 1; x < m_Player->m_GameGrid->GetRows() - 1; x++)
		{
			// Searhing for the same coordinates as chosen GridEntity so 

			Entity gridentity = m_Player->m_GameGrid->m_GameGrid[y][x];

			auto& trans = gridentity.GetComponent<TransformComponent>().Translation;
			// Condition below is searching for the same entity translation coordinates as the bomb.
			// So it can properly know where spread wings should be
			if (trans == this->translation()) 
			{ 
				// Every wing has its own vector, ergo for loop and switch
				// This for loop is needed so the bomb can have longer wings, ergo wider explosions

				// Left wing
				// #m_Properties.Reach is just a variable indicating the power of the placed bomb
				// Pre-checking if there can be another entity with explosion animation
				for (uint32_t j = 0; j < m_Properties.Reach; j++) 
				{
					// #l_index is a distance  
					int l_index = x - (j + 1);
					if (l_index >= 1)
					{
						// Grid Entity
						Entity left = m_Player->m_GameGrid->m_GameGrid[y][l_index];
						// Getting GridEntity coordinates so it can create new entity 
						// And eventually adding it to the wing #vector
						auto& trans_l = left.GetComponent<TransformComponent>().Translation;

						// Checking the of the GridEntity
						switch (left.GetComponent<EntityTypeComponent>().Type)
						{
							// Breakable
							case EntityType::TILE_BOX:
							{
								// Adding it to another separated #m_Destroyed vector
								// Every GridEntity in this vector will be destroyed ( Not the ECS entity )
								m_Destroyed.push_back(left);

								// When single destoyable wall ( box ) will be destroyed by two bomb at once,
								// the second bomb will not add spread explosion entity because, it will not be
								// yet destroyed( marked as TILE_EMPTY ) 
								// This line is necessary and prevents this bug
								left.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
								goto LEFT_BREAKOUT;
								break;
							}
							case EntityType::TILE_WALL: // THERE IS AN OBSTACLE SO STOP WITH EXPANDING
								goto LEFT_BREAKOUT;
								break;
							case EntityType::TILE_EMPTY:
								{
									Entity spread = m_GameScene->CreateEntityWithDrawOrder(3, "LeftWingEntity");
									initSpread(spread);
									spread.GetComponent<TransformComponent>().Translation = trans_l;
									m_LeftWing.push_back(spread);
								}
								break;
						}
					}
				}
			LEFT_BREAKOUT:
				for (size_t i = 0; i < m_LeftWing.size(); i++)
				{
					m_LeftWing[i].GetComponent<TransformComponent>().Rotation.z = glm::radians(90.0f);
					auto& fa = m_LeftWing[i].AddCustomComponent<FrameAnimator>(m_LeftWing[i]);
					if (i == m_LeftWing.size() - 1)
					{
						fa.Add(m_Player->m_BombSpreadEndWingAnimation);
						fa.Switch("BombEndWingExplosion");
						break;
					}
					fa.Add(m_Player->m_BombSpreadWingAnimation);
					fa.Switch("BombWingExplosion");
				}
				// UP
				for (uint32_t j = 0; j < m_Properties.Reach; j++) // every single wing
				{
					int u_index = y - (j + 1); 
					if (u_index >= 1)
					{
						Entity up = m_Player->m_GameGrid->m_GameGrid[u_index][x];
						switch (up.GetComponent<EntityTypeComponent>().Type)
						{
							case EntityType::TILE_BOX:
							{
								m_Destroyed.push_back(up);
								up.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
							}
							case EntityType::TILE_WALL:
								goto UP_BREAKOUT; 
								break;
							case EntityType::TILE_EMPTY:
								{
									Entity spread = m_GameScene->CreateEntityWithDrawOrder(3, "UpWingEntity");
									initSpread(spread);
									spread.GetComponent<TransformComponent>().Translation = up.GetComponent<TransformComponent>().Translation;
									m_UpWing.push_back(spread);
								}
								break;
						}
					}
				}

			UP_BREAKOUT:
				for (size_t i = 0; i < m_UpWing.size(); i++)
				{
					auto& fa = m_UpWing[i].AddCustomComponent<FrameAnimator>(m_UpWing[i]);
					if (i == m_UpWing.size() - 1)
					{
						fa.Add(m_Player->m_BombSpreadEndWingAnimation);
						fa.Switch("BombEndWingExplosion");
						break;
					}
					fa.Add(m_Player->m_BombSpreadWingAnimation);
					fa.Switch("BombWingExplosion");
				}

				// RIGHT 
				for (uint32_t j = 0; j < m_Properties.Reach; j++) // every single wing
				{
					int r_index = x + (j + 1); 
					if (r_index < m_Player->m_GameGrid->GetColumns())
					{
						Entity right = m_Player->m_GameGrid->m_GameGrid[y][r_index];
						switch (right.GetComponent<EntityTypeComponent>().Type)
						{
						case EntityType::TILE_BOX:
						{
							m_Destroyed.push_back(right);
							right.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
						}
						case EntityType::TILE_WALL:
							goto RIGHT_BREAKOUT;
							break;
						case EntityType::TILE_EMPTY:
							{
								Entity spread = m_GameScene->CreateEntityWithDrawOrder(3, "RightWingEntity");
								initSpread(spread);
								spread.GetComponent<TransformComponent>().Translation = right.GetComponent<TransformComponent>().Translation;
								m_RightWing.push_back(spread);
							}
							break;
						}
					}
				}
			RIGHT_BREAKOUT:
				for (size_t i = 0; i < m_RightWing.size(); i++)
				{
					m_RightWing[i].GetComponent<TransformComponent>().Rotation.z = glm::radians(270.0f);
					auto& fa = m_RightWing[i].AddCustomComponent<FrameAnimator>(m_RightWing[i]);
					if (i == m_RightWing.size() - 1)
					{
						fa.Add(m_Player->m_BombSpreadEndWingAnimation);
						fa.Switch("BombEndWingExplosion");
						break;
					}
					fa.Add(m_Player->m_BombSpreadWingAnimation);
					fa.Switch("BombWingExplosion");
				}
				// DOWN
				for (uint32_t j = 0; j < m_Properties.Reach; j++) // every single wing
				{
					int d_index = y + (j + 1); 
					if (d_index < m_Player->m_GameGrid->GetRows())
					{
						Entity down = m_Player->m_GameGrid->m_GameGrid[d_index][x];
						switch (down.GetComponent<EntityTypeComponent>().Type)
						{	
						case EntityType::TILE_BOX:
						{
							m_Destroyed.push_back(down);
							down.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
						}
						case EntityType::TILE_WALL:
							goto DOWN_BREAKOUT;
							break;
						case EntityType::TILE_EMPTY:
							{
								Entity spread = m_GameScene->CreateEntityWithDrawOrder(3, "DownWingEntity");
								initSpread(spread);
								spread.GetComponent<TransformComponent>().Translation = down.GetComponent<TransformComponent>().Translation;
								m_DownWing.push_back(spread);
							}

							break;
						}
					}
				}
			DOWN_BREAKOUT:
				for (size_t i = 0; i < m_DownWing.size(); i++)
				{
					m_DownWing[i].GetComponent<TransformComponent>().Rotation.z = glm::radians(180.0f);
					auto& fa = m_DownWing[i].AddCustomComponent<FrameAnimator>(m_DownWing[i]);
					if (i == m_DownWing.size() - 1)
					{
						fa.Add(m_Player->m_BombSpreadEndWingAnimation);
						fa.Switch("BombEndWingExplosion");
						break;
					}
					fa.Add(m_Player->m_BombSpreadWingAnimation);
					fa.Switch("BombWingExplosion");
				}
			}
		}
	}

	Entity center = m_GameScene->CreateEntityWithDrawOrder(3, "CenterWingEntity");
	center.GetComponent<TransformComponent>().Translation = this->translation();

	center.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
	center.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;

	auto& fa = center.AddCustomComponent<FrameAnimator>(center);
	fa.Add(m_Player->m_BombSpreadCenterAnimation);
	fa.Switch("BombCenterExplosion");

	m_Properties.SpreadEntities["CenterEntity"] = std::vector<Entity>{ center };
	m_Properties.SpreadEntities["LeftWing"] = m_LeftWing;
	m_Properties.SpreadEntities["UpWing"] = m_UpWing;
	m_Properties.SpreadEntities["RightWing"] = m_RightWing;
	m_Properties.SpreadEntities["DownWing"] = m_DownWing;


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
		for (auto& other : m_Player->m_PlacedBombs)
		{
			for (auto& wings : m_Properties.SpreadEntities) // Spread // BASIC BOMB MECHANIC IMPLEMENTED ( ??? ) ROOM FOR TESTING
			{
				for (auto& entity : wings.second /* VECTOR */)
				{
					if (other.m_Properties.State != BombState::EXPLOSION &&
						other.GetBombEntity().GetComponent<TransformComponent>().Translation == entity.GetComponent<TransformComponent>().Translation)
					{
						// TODO: Fix animations overlapping each other
						entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
						other.Explode();
					}
				}
			}
		}

		m_Entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		// Setting alpha value to 1 
		for (auto& wings : m_Properties.SpreadEntities) 
		{
			for (auto& entity : wings.second /* VECTOR */)
			{
				entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}
		}
		// Destroying boxes in the way
		for (auto& tobedestroyed : m_Destroyed) 
		{
			tobedestroyed.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");
		}
	}

	if (m_Properties.State == BombState::EXPLOSION)
	{
		for (auto& wings : m_Properties.SpreadEntities) // Explosion
		{
			for (auto& entity : wings.second /* VECTOR */)
			{
				auto& frameanimator = entity.GetComponent<FrameAnimator>();
				frameanimator.OnUpdate(ts);
			}
			for (auto& entity : wings.second /* VECTOR */)
			{
				auto& frameanimator = entity.GetComponent<FrameAnimator>();
				if (!frameanimator.IsAnyPlaying())
				{
					DestroyItSelf();
					return true; // Animation finally complete and can be safely removed
				}
			}
		}
	}
	return false;
}

void Bomb::DestroyItSelf()
{
	// Destroys all entities related to bomb

	m_GameScene->DestroyEntity(m_Entity);

	for (auto& wings : m_Properties.SpreadEntities) 
	{
		for (auto& entity : wings.second)
		{
			m_GameScene->DestroyEntity(entity);
		}
	}

	m_Properties.SpreadEntities.clear();
}

void Bomb::Explode()
{
	m_ChainExplosion = true;
}

