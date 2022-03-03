 #include "Grid.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Game/Enemy.hpp"

#include <Hurikan/Scene/Components.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>
#include <box2d/b2_body.h>

Scope<Entity[]> Grid::m_Grid;

extern GameData g_InGameData;

Grid::~Grid()
{
	//m_Grid.release();
	m_Grid.reset();
}

/*

	# -> Unbreakable Wall
	B -> Breakable Wall
	M -> Regular Monster
	F -> Fast Monster
	T -> Smart monster
	S -> Boss Monster
	U -> Exit door under wall
	P -> Player start position
	K -> Key(always under breakable wall)
	W -> Bomb power upgrade
	D -> Player speed upgrade
	O -> Bomb count upgrade
	- -> Empty space
*/

void Grid::Create(Scene* const scene)
{
	g_GameScene = scene;

	m_Grid = CreateScope<Entity[]>(GetLevelWidth() * GetLevelHeight());

	constexpr float offset_x = -15.5f;

	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			auto& gridEntity = g_GameScene->CreateEntityWithDrawOrder(2);
			m_Grid[y * GetLevelWidth() + x] = gridEntity;

			gridEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
			gridEntity.GetComponent<TransformComponent>().Translation = { x + offset_x, (y + 1.5f - GetLevelHeight() / 2) * (-1), 0.0f };
			gridEntity.AddComponent<Rigidbody2DComponent>();
			gridEntity.AddComponent<BoxCollider2DComponent>();
			auto& gnc = gridEntity.AddCustomComponent<GridNodeComponent>();
			gnc.Index = { x, y };

			switch (SaveLoadSystem::GetCurrentLevel().MapSkeleton[y * GetLevelWidth() + x])
			{
			case '#':
			{
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("Wall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				gnc.Obstacle = true;
				break;
			}
			case 'B': 
			{
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BREAKABLE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::GetAnimation("WallBreakAnimation"));

				gnc.Obstacle = true;
				break;
			}
			case 'K':
			{
				{
					auto& loot = g_GameScene->CreateEntityWithDrawOrder(1);
					loot.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("Key");
					loot.Transform().Translation = gridEntity.Transform().Translation;
					auto& lc = gridEntity.AddCustomComponent<LootComponent>(Loot::KEY);
					lc.LootHandle = loot;
					lc.Obtainable = false;
				}

				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BREAKABLE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::GetAnimation("WallBreakAnimation"));

				gnc.Obstacle = true;

				m_KeyGridEntity = gridEntity;
				break;
			}
			case 'U':
			{
				{
					auto& exit = scene->CreateEntityWithDrawOrder(1);
					exit.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("ExitDoor");
					exit.Transform().Translation = gridEntity.Transform().Translation;
					gridEntity.AddCustomComponent<LootComponent>(Loot::EXIT).LootHandle = exit;
					gridEntity.GetComponent<LootComponent>().Obtainable = true;
				}

				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BREAKABLE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::GetAnimation("WallBreakAnimation"));

				gnc.Obstacle = true;
				break;
			}

			case 'P': // PLAYER SPAWN POINT
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				m_PlayerGridPosition = m_Grid[y * GetLevelWidth() + x];
				Dispatch(GameEventType::PLAYER_START_POSITION, gridEntity.Transform().Translation);
				break;
			}
			case 'M': // REGULAR MONSTER SPAWN POINT
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::ENEMY_REGULAR;
				Dispatch(GameEventType::ENEMY_CREATE_NEW, gridEntity); // Event for enemy spawner
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
				break;
			}
			case 'F': // FAST MONSTER SPAWN POINT
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::ENEMY_FAST;
				Dispatch(GameEventType::ENEMY_CREATE_NEW, gridEntity); // Event for enemy spawner
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
				break;
			}
			case 'T': // Smart enemy
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::ENEMY_SMART;
				Dispatch(GameEventType::ENEMY_CREATE_NEW, gridEntity); // Event for enemy spawner
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
				break;
			}
			case 'D': // Player speed upgrade
			{
				{
					auto& speed = scene->CreateEntityWithDrawOrder(1);
					speed.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("SpeedUpgrade");
					speed.Transform().Translation = gridEntity.Transform().Translation;
					gridEntity.AddCustomComponent<LootComponent>(Loot::SPEED_UPGRADE).LootHandle = speed;
					gridEntity.GetComponent<LootComponent>().Obtainable = true;
				}

				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BREAKABLE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::GetAnimation("WallBreakAnimation"));

				gnc.Obstacle = true;
				break;
			}
			case 'W': // Player bomb power upgrade
			{
				{
					auto& power = scene->CreateEntityWithDrawOrder(1);
					power.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("PowerUpgrade");
					power.Transform().Translation = gridEntity.Transform().Translation;
					gridEntity.AddCustomComponent<LootComponent>(Loot::BOMB_UPGRADE_POWER).LootHandle = power;
					gridEntity.GetComponent<LootComponent>().Obtainable = true;
				}

				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BREAKABLE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::GetAnimation("WallBreakAnimation"));

				gnc.Obstacle = true;
				break;
			}
			case 'O': // Player bomb count upgrade
			{
				{
					auto& count = scene->CreateEntityWithDrawOrder(1);
					count.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("BombCountUpgrade");
					count.Transform().Translation = gridEntity.Transform().Translation;
					gridEntity.AddCustomComponent<LootComponent>(Loot::BOMB_UPGRADE_COUNT).LootHandle = count;
					gridEntity.GetComponent<LootComponent>().Obtainable = true;
				}

				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::BREAKABLE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::GetAnimation("WallBreakAnimation"));

				gnc.Obstacle = true;
				break;
			}

			case '-': // Empty
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				break;
			case 'S':
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::ENEMY_BOSS;
				Dispatch(GameEventType::ENEMY_CREATE_NEW, gridEntity); // Event for enemy spawner
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
				break;
			}
			default:
				HU_CORE_ASSERT(false, "Invalid tile! {0}", SaveLoadSystem::GetCurrentLevel().MapSkeleton[y * GetLevelWidth() + x]);
				break;
			}
		}
	}

	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			auto& gnc = m_Grid[y * GetLevelWidth() + x].GetComponent<GridNodeComponent>();

			if (y > 0)
			{
				gnc.Neighbours.push_back(m_Grid[(y - 1) * GetLevelWidth() + x]);
			}
			if (x > 0)
			{
				gnc.Neighbours.push_back(m_Grid[y * GetLevelWidth() + x - 1]);
			}
			if (y < GetLevelHeight() - 1)
			{
				gnc.Neighbours.push_back(m_Grid[(y + 1) * GetLevelWidth() + x]);
			}
			if (x < GetLevelWidth() - 1)
			{
				gnc.Neighbours.push_back(m_Grid[y * GetLevelWidth() + x + 1]);
			}
		}
	}

	Dispatch(GameEventType::PLAYER_CHANGED_GRID_POSITION, m_PlayerGridPosition);
}

Entity& Grid::Get(unsigned int y, unsigned int x)
{
	return m_Grid[y * SaveLoadSystem::GetCurrentLevel().Width + x];
}

const int Grid::GetLevelHeight() const
{
	return SaveLoadSystem::GetCurrentLevel().Height;
}

const int Grid::GetLevelWidth() const
{
	return SaveLoadSystem::GetCurrentLevel().Width;
}

void Grid::ClearNodes()
{
	int size = SaveLoadSystem::GetCurrentLevel().Width * SaveLoadSystem::GetCurrentLevel().Height;

	for (int i = 0; i < size; ++i) // TODO: Optimize
	{
		auto& node = m_Grid[i].GetComponent<GridNodeComponent>();

		node.Visited = false;
		node.GlobalGoal = INFINITY;
		node.LocalGoal = INFINITY;
		node.Parent = Entity{};
	}
}

void Grid::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::PLAYER_MOVED)
	{
		static bool b = false;

		if(!b && Input::IsKeyPressed(Key::C))
		{
			b = true;
			Dispatch(GameEventType::PLAYER_SUCCESS_EXIT);
		}

		auto& player_position = std::any_cast<glm::vec3>(e.Data);

		for (int i = 0; i < GetLevelWidth() * GetLevelHeight(); ++i)
		{
			auto& grid_tr = m_Grid[i].Transform().Translation;

			if (m_Grid[i] != m_PlayerGridPosition && Utils::Intersect(player_position, grid_tr))
			{
				m_PlayerGridPosition = m_Grid[i];

				if (m_PlayerGridPosition.HasComponent<LootComponent>())
				{
					if (m_PlayerGridPosition.GetComponent<LootComponent>().Type == Loot::KEY && m_PlayerGridPosition.GetComponent<LootComponent>().Obtainable)
					{
						Dispatch(GameEventType::KEY_OBTAINED);
						m_KeyObtained = true;
					}
					else if (m_PlayerGridPosition.GetComponent<LootComponent>().Type == Loot::EXIT)
					{
						if (m_KeyObtained) {
							// Won
							// Play some nice winning music
							Dispatch(GameEventType::PLAYER_SUCCESS_EXIT);
							return;
						}
						else {
							// Display -> "You need to find a key first!"
							Dispatch(GameEventType::DISPLAY_KEY_FIRST);
							return;
						}

					}
					else if (m_PlayerGridPosition.GetComponent<LootComponent>().Type == Loot::BOMB_UPGRADE_POWER)
					{
						Dispatch(GameEventType::PLAYER_POWER_UPGRADE);
					}
					else if (m_PlayerGridPosition.GetComponent<LootComponent>().Type == Loot::BOMB_UPGRADE_COUNT)
					{
						Dispatch(GameEventType::PLAYER_BOMB_COUNT_UPGRADE);
					}
					else if (m_PlayerGridPosition.GetComponent<LootComponent>().Type == Loot::SPEED_UPGRADE)
					{
						Dispatch(GameEventType::PLAYER_SPEED_UPGRADE);
					}


					g_GameScene->DestroyEntity(m_PlayerGridPosition.GetComponent<LootComponent>().LootHandle);
					m_PlayerGridPosition.RemoveComponent<LootComponent>();
					

				}

				Dispatch(GameEventType::PLAYER_CHANGED_GRID_POSITION, m_PlayerGridPosition);
				return;
			} 
		}
	}
	else if (e.Type == GameEventType::BOMB_PLACED) 
	{
		// Bomb is placed so the entity under the bomb is now occupied

		auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);

		HU_CORE_ASSERT(GRID_ENTITY, "Entity is not valid!");

		GRID_ENTITY.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB;
		GRID_ENTITY.GetComponent<GridNodeComponent>().Obstacle = true;
	}
	else if (e.Type == GameEventType::BOMB_EXPLODED)
	{
	}
	else if (e.Type == GameEventType::BOMB_VANISHED)
	{
		// Bomb exploded so the GRID ENTITY under the bomb is cleared for future uses

		auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);

		HU_CORE_ASSERT(GRID_ENTITY, "Entity is not valid!");

		GRID_ENTITY.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
		GRID_ENTITY.GetComponent<GridNodeComponent>().Obstacle = false;
	}
	else if (e.Type == GameEventType::WALL_BREAK)
	{
		auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);
		GRID_ENTITY.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
		GRID_ENTITY.GetComponent<Animator>().Play("WallBreakAnimation");
		auto rb = (b2Body*)GRID_ENTITY.GetComponent<Rigidbody2DComponent>().RuntimeBody;
		rb->SetEnabled(false);
		GRID_ENTITY.GetComponent<GridNodeComponent>().Obstacle = false;

		m_AnimationQueue.emplace_back(GRID_ENTITY);

		if (GRID_ENTITY.HasComponent<LootComponent>() 
			&& GRID_ENTITY.GetComponent<LootComponent>().Obtainable && 
			GRID_ENTITY.GetComponent<LootComponent>().Type != Loot::KEY)
		{
			GRID_ENTITY.GetComponent<LootComponent>().LootHandle.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
		}
	}
}


void Grid::OnUpdate(Timestep& ts)
{
	const auto& value = 1000 * (SaveLoadSystem::GetGameData().CompletedLevels + 1);

	if (m_KeyGridEntity && m_KeyGridEntity.HasComponent<LootComponent>())
			if(g_InGameData.Score >= value && !m_KeyGridEntity.GetComponent<LootComponent>().Obtainable)
			{
				m_KeyGridEntity.GetComponent<LootComponent>().Obtainable = true;
				m_KeyGridEntity.GetComponent<LootComponent>().LootHandle.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}

	for (size_t i = 0; i < m_AnimationQueue.size(); i++)
	{
		auto& e = m_AnimationQueue[i].GetComponent<Animator>();
		e.OnUpdate(ts);
		if (!e.IsAnyPlaying())
		{
			m_AnimationQueue.erase(m_AnimationQueue.begin() + i);
		}
	}
}