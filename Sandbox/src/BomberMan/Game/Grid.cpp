 #include "Grid.h"

#include "BomberMan/Core/ResourceManager.h"
#include "BomberMan/Game/Enemy.h"
#include "BomberMan/Core/SaveManager.h"

#include <Hurikan/Scene/Components.h>
#include <box2d/b2_body.h>

Scope<Entity[]> Grid::m_Grid;

void Grid::Create(Scene& scene)
{
	// Load level from LevelManager

	m_CurrentLevel = LevelManager::GetCurrentLevel();

	m_Grid = CreateScope<Entity[]>(GetLevelWidth() * GetLevelHeight());

	float offset_x = -15.5f;

	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			auto& gridEntity = scene.CreateEntityWithDrawOrder(1, "Empty");
			m_Grid[y * GetLevelWidth() + x] = gridEntity;

			gridEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
			gridEntity.GetComponent<TransformComponent>().Translation = { x + offset_x, (y + 1.5f - GetLevelHeight() / 2) * (-1), 0.0f };
			gridEntity.AddComponent<Rigidbody2DComponent>();
			gridEntity.AddComponent<BoxCollider2DComponent>();
			auto& gnc = gridEntity.AddCustomComponent<GridNodeComponent>();
			gnc.Index = { x, y }; // Useless ?

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
			case 'B': // BREAKABLE WALL
			{
				gridEntity.GetComponent<TagComponent>().Tag = "Box";
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("BreakableWall");
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::LOOT_WALL;
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
				DispatchToAll(GameEventType::VALUE_PLAYER_START_POS, gridEntity.Transform().Translation);
				break;
			}
			case 'M': // MONSTER SPAWN POINT
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				auto& message = EnemyProps(gridEntity, EnemyType::REGULAR);
				DispatchToAll(GameEventType::CREATE_NEW_ENEMY, message); // Event for enemy spawner
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
	m_Grid[0].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;

	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			auto& gnc = m_Grid[y * GetLevelWidth() + x].GetComponent<GridNodeComponent>();

			if (y > 0)
				gnc.Neighbours.push_back(m_Grid[(y - 1) * GetLevelWidth() + x]);
			if (x > 0)
				gnc.Neighbours.push_back(m_Grid[y * GetLevelWidth() + x - 1]);
			if (y < GetLevelHeight() - 1)
				gnc.Neighbours.push_back(m_Grid[(y + 1) * GetLevelWidth() + x]);
			if (x < GetLevelWidth() - 1)
				gnc.Neighbours.push_back(m_Grid[y * GetLevelWidth() + x + 1]);
		}
	}

	DispatchToAll(GameEventType::VALUE_PLAYER_CHNG_POS_GRID, m_PlayerGridPosition);
}

void Grid::ClearNodes()
{
	int size = LevelManager::GetCurrentLevel().Width * LevelManager::GetCurrentLevel().Height;

	for (int i = 0; i < size; ++i) // TODO: Optimize
	{
		auto& node = m_Grid[i].GetComponent<GridNodeComponent>();

		/*if (m_Grid[i].GetComponent<EntityTypeComponent>().Type == EntityType::EMPTY)
		{
			if (!m_Grid[i].HasComponent<SpriteRendererComponent>())
			{
				m_Grid[i].AddComponent<SpriteRendererComponent>().Color = { 1,1,1,0.0f }; // TODO: MINOR FIX
			}
			else
			{
				m_Grid[i].GetComponent<SpriteRendererComponent>().Color = { 1,1,1,0.0f };
			}
		}*/

		node.Visited = false;
		node.GlobalGoal = INFINITY;
		node.LocalGoal = INFINITY;
		node.Parent = Entity{};
	}
}

static inline bool intersect(const glm::vec3& a, const glm::vec3& b)
{
	return a.x > b.x - 0.5f && a.x < b.x + 0.5f && 
		   a.y > b.y - 0.5f && a.y < b.y + 0.5f;
}

void Grid::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::VALUE_PLAYER_MOVING)
	{
		auto& player_position = std::any_cast<glm::vec3>(e.Data);

		for (int i = 0; i < GetLevelWidth() * GetLevelHeight(); ++i)
		{
			auto& grid_tr = m_Grid[i].Transform().Translation;

			if (m_Grid[i] != m_PlayerGridPosition && intersect(player_position, grid_tr))
			{
				m_PlayerGridPosition = m_Grid[i];

				HU_INFO("x: {0} | y: {1}", player_position.x, player_position.y)

				DispatchToAll(GameEventType::VALUE_PLAYER_CHNG_POS_GRID, m_PlayerGridPosition);

				return;
			}
		}
	}
	else if (e.Type == GameEventType::BOMB_PLACED) 
	{
		// Bomb is placed so the @e under the bomb is now occupied

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
	else if (e.Type == GameEventType::BREAK_WALL)
	{
		auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);

		GRID_ENTITY.GetComponent<Animator>().Play("WallBreakAnimation");
		GRID_ENTITY.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;
		auto rb = (b2Body*)GRID_ENTITY.GetComponent<Rigidbody2DComponent>().RuntimeBody;
		rb->SetEnabled(false);
		GRID_ENTITY.GetComponent<GridNodeComponent>().Obstacle = false;

		m_AnimationQueue.emplace_back(GRID_ENTITY);

		SaveManager::Data().Score += 100;
	}
}


void Grid::OnUpdate(Timestep& ts)
{
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