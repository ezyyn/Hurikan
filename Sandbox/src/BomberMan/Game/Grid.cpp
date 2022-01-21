#include "Grid.h"

#include "BomberMan/Core/ResourceManager.h"

#include <Hurikan/Scene/Components.h>

Hurikan::Scope<Entity[]> Grid::m_Grid;

void Grid::Create(Scene& scene)
{
	// Load level from LevelManager

	m_CurrentLevel = LevelManager::GetCurrentLevel();

	m_Grid = CreateScope<Entity[]>(GetLevelWidth() * GetLevelHeight());

	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			auto& gridEntity = scene.CreateEntityWithDrawOrder(1, "Empty");
			m_Grid[y * GetLevelWidth() + x] = gridEntity;

			gridEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
			gridEntity.GetComponent<TransformComponent>().Translation = { x, (y - GetLevelHeight() / 2) * (-1), 0.0f };
			gridEntity.AddComponent<Rigidbody2DComponent>();
			gridEntity.AddComponent<BoxCollider2DComponent>();
			auto& gnc = gridEntity.AddCustomComponent<GridNodeComponent>();
			gnc.Index = { x, y }; // Useless ?

			if (y > 0)
				gnc.Neighbours.emplace_back(m_Grid[(y - 1) * GetLevelWidth() + x]);
			if (x > 0)
				gnc.Neighbours.emplace_back(m_Grid[y * GetLevelWidth() + x - 1]);
			if (y < GetLevelHeight() - 1)
				gnc.Neighbours.emplace_back(m_Grid[(y + 1) * GetLevelWidth() + x]);
			if (x < GetLevelWidth() - 1)
				gnc.Neighbours.emplace_back(m_Grid[y * GetLevelWidth() + x + 1]);

			switch (m_CurrentLevel.MapSkeleton[y * GetLevelWidth() + x])
			{
			case '#': // WALL
			{
				gridEntity.GetComponent<TagComponent>().Tag = "Wall";
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("Wall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				gnc.Obstacle = true;
				break;
			}
			case 'B': // LOOT BOX 
			{
				gridEntity.GetComponent<TagComponent>().Tag = "Box";
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::LOOT_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				/*auto& fa = gridEntity.AddCustomComponent<Animator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceLoader::RequestAnimation("WallBreakAnimation"));*/

				gnc.Obstacle = true;
				m_DestroyableWalls.emplace_back(gridEntity);
				break;
			}
			case 'P': // PLAYER SPAWN POINT
			{
				//startpos->x = gridEntity.Transform().Translation.x;
				//startpos->y = gridEntity.Transform().Translation.y;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				m_PlayerGridPosition = m_Grid[y * GetLevelWidth() + x];
				DispatchToAll(GameEventType::VALUE_PLAYER_START_POS, gridEntity.Transform().Translation);
				DispatchToAll(GameEventType::VALUE_PLAYER_CHNG_POS_GRID, m_PlayerGridPosition);
				break;
			}
			case 'M': // MONSTER SPAWN POINT
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;

				//MonsterProperties props;
				//props.StartPosition = gridEntity.Transform().Translation;
				//m_EnemySpawner.Spawn(props);
				break;
			}
			case '-': // EMPTY
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				break;
			default:
				HU_CORE_ASSERT(false, "Invalid tile!");
				break;
			}
		}
	}
}

void Grid::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::VALUE_PLAYER_MOVING)
	{
		auto& player_position = glm::round(std::any_cast<glm::vec3>(e.Data));

		int size = GetLevelWidth() * GetLevelHeight();
		for (int i = 0; i < size; ++i)
		{
			if (player_position == m_Grid[i].Transform().Translation)
			{
				if (m_Grid[i] == m_PlayerGridPosition)
					return;

				m_PlayerGridPosition = m_Grid[i];

				DispatchToAll(GameEventType::VALUE_PLAYER_CHNG_POS_GRID, m_PlayerGridPosition);

			//	HU_INFO("x: {0} | y: {1}", player_position.x, player_position.y)
				return;
			}
		}
	}
	else if (e.Type == GameEventType::BOMB_PLACED)
	{
		auto& entity = std::any_cast<Entity>(e.Data);

		HU_CORE_ASSERT(entity, "Entity is not valid!");

		entity.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB;
		entity.GetComponent<GridNodeComponent>().Obstacle = true;
	}
}