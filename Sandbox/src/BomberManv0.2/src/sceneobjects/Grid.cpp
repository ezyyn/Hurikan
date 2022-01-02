#include "Grid.h"

#include "../sceneobjects/Player.h"

#include <box2d/b2_body.h>

extern Player* g_Player;
extern Scene* g_GameScene;

void Grid::Init()
{
	Ref<Texture2D> m_SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");
	m_BackgroundTile = Texture2D::Create("assets/textures/background.png");

	m_WallTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 0 }, { 16, 16 });
	m_BoxTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 0 }, { 16, 16 });
}

void Grid::Generate(const Level& level, glm::vec2* const startpos)
{
	m_CurrentLevel = level;
	
	// Every level could have unique textures
	//Entity backgroundEntity = g_GameScene->CreateEntityWithDrawOrder(0, "Gamegrid-Background");
	//backgroundEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = m_BackgroundTile;
	//backgroundEntity.GetComponent<TransformComponent>().Scale = { m_CurrentLevel.Width - 2, m_CurrentLevel.Width - 2 , 0.0f };
	//backgroundEntity.GetComponent<TransformComponent>().Translation = { glm::round(m_CurrentLevel.Width / 2), -glm::round(m_CurrentLevel.Height / 2) - 1, 0 };
	//backgroundEntity.GetComponent<SpriteRendererComponent>().TilingFactor = (float)m_CurrentLevel.Width;

	m_GameGrid = new GridNode[GetLevelWidth() * GetLevelHeight()];

	//for (int i = 0; i < GetLevelHeight(); ++i)
	//	m_GameGrid[i] = new GridNode[GetLevelWidth()];

	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			Entity gridEntity = g_GameScene->CreateEntityWithDrawOrder(1, "Empty");
			gridEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
			gridEntity.GetComponent<TransformComponent>().Translation = { x, (y - GetLevelHeight() / 2) * (-1), 0.0f };
			gridEntity.AddComponent<Rigidbody2DComponent>();
			gridEntity.AddComponent<BoxCollider2DComponent>();

			GridNode& current = m_GameGrid[y * GetLevelWidth() + x];

			current.Handle = gridEntity;
			current.IndexX = x;
			current.IndexY = y;

			// Counter-clockwise
			if (y > 0)
				current.Neighbours.push_back(&m_GameGrid[(y - 1) * GetLevelWidth() + x]);
			if (x > 0)
				current.Neighbours.push_back(&m_GameGrid[y * GetLevelWidth() + x - 1]);
			if (y < GetLevelHeight() - 1)
				current.Neighbours.push_back(&m_GameGrid[(y + 1) * GetLevelWidth() + x]);
			if (x < GetLevelWidth() - 1)
				current.Neighbours.push_back(&m_GameGrid[y * GetLevelWidth() + x + 1]);

			switch (m_CurrentLevel.MapSkeleton[y * GetLevelWidth() + x])
			{
			case '#': // WALL
			{
				gridEntity.GetComponent<TagComponent>().Tag = "Wall";
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = m_WallTile;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;

				current.Obstacle = true;
				break;
			}
			case 'B': // LOOT BOX 
			{
				gridEntity.GetComponent<TagComponent>().Tag = "Box";
				gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = m_BoxTile;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_BOX;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				auto& fa = gridEntity.AddCustomComponent<FrameAnimator>();
				fa.SetTarget(gridEntity);
				fa.Add(ResourceManager::RequestAnimation("WallBreakAnimation"));

				m_GameGrid[y * GetLevelWidth() + x].Obstacle = true;
				m_Destroyable.push_back(&m_GameGrid[y * GetLevelWidth() + x]);
				break;
			}
			case 'P': // PLAYER SPAWN POINT
			{
				startpos->x = gridEntity.Transform().Translation.x;
				startpos->y = gridEntity.Transform().Translation.y;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				m_PlayerGridPosition = &m_GameGrid[y * GetLevelWidth() + x];
				break;
			}
			case 'M': // MONSTER SPAWN POINT
			{
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;

				MonsterProperties props;
				props.StartPosition = gridEntity.Transform().Translation;
				m_EnemySpawner.Spawn(props);
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
/// <summary>
/// Helper method for iterating through the grid
/// </summary>
void Grid::Each(const std::function<bool(GridNode*)>& func)
{
	for (int y = 0; y < GetLevelHeight(); ++y)
	{
		for (int x = 0; x < GetLevelWidth(); ++x)
		{
			if (func(&m_GameGrid[y * GetLevelWidth() + x]))
				return;
		}
	}
}

GridNode* Grid::SearchFor(Entity entity)
{
	for (int y = 1; y < GetLevelHeight() - 1; ++y)
	{
		for (int x = 1; x < GetLevelWidth() - 1; ++x)
		{
			if (m_GameGrid[y * GetLevelWidth() + x].Handle.Transform().Translation.x == glm::round(entity.Transform().Translation.x) &&
				m_GameGrid[y * GetLevelWidth() + x].Handle.Transform().Translation.y == glm::round(entity.Transform().Translation.y))
					return &m_GameGrid[y * GetLevelWidth() + x];
		}
	}
	return nullptr;
}

GridNode* Grid::SearchFor(const glm::vec3& position)
{
	for (int y = 1; y < GetLevelHeight() - 1; ++y)
	{
		for (int x = 1; x < GetLevelWidth() - 1; ++x)
		{
			if (m_GameGrid[y * GetLevelWidth() + x].Handle.Transform().Translation.x == glm::round(position.x) &&
				m_GameGrid[y * GetLevelWidth() + x].Handle.Transform().Translation.y == glm::round(position.y))
				return &m_GameGrid[y * GetLevelWidth() + x];
		}
	}
	return nullptr;
}

void Grid::OnUpdate(Timestep ts)
{
	for (size_t i = 0; i < m_Destroyable.size(); ++i)
	{
		auto& fa = m_Destroyable[i]->Handle.GetComponent<FrameAnimator>();

		if (fa.IsAnyPlaying())
		{
			fa.OnUpdate(ts);
			if (!fa.IsAnyPlaying())
			{
				m_Destroyable[i]->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				m_Destroyable[i]->Handle.RemoveComponent<SpriteRendererComponent>();
				m_Destroyable[i]->Obstacle = false;

				b2Body* body = (b2Body*)m_Destroyable[i]->Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody;
				body->SetEnabled(false);
				m_Destroyable[i]->Handle.RemoveComponent<FrameAnimator>();

				m_Destroyable.erase(m_Destroyable.begin() + i);
			}
		}
	}

	// Checks every frame for player to move on the grid
	if(g_Player->IsMoving())
		m_PlayerGridPosition = SearchFor(g_Player->Position2());
	
	m_EnemySpawner.OnUpdate(ts);

	// AI STUFF
	if (m_PlayerPreviousPosition != m_PlayerGridPosition || m_BombEvent)
	{
		HU_INFO("Player position changed! Calculating new path.");
		m_BombEvent = false;
		m_PlayerPreviousPosition = m_PlayerGridPosition;

		m_EnemySpawner.UpdatePaths();
	}

}

void Grid::Shutdown()
{
	delete[] m_GameGrid;

	m_GameGrid = nullptr;
}
