#include "GameGrid.h"

#include "../core/GameComponents.h"
#include "../scripts/FrameAnimatorScript.h"

#include "Player.h"

void GameGrid::Init(Scene* scene, Player* player, uint32_t rows, uint32_t columns)
{
	g_GameScene = scene;
	g_Player = player;

	m_Rows = rows;
	m_Columns = columns;

	m_SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");
	Ref<Texture2D> m_BackgroundTile = Texture2D::Create("assets/textures/background.png");
	Ref<SubTexture2D> m_WallTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 0 }, { 16, 16 });
	Ref<SubTexture2D> m_BoxTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 0 }, { 16, 16 });

	Entity backgroundEntity = scene->CreateEntityWithDrawOrder(0, "Gamegrid-Background");
	backgroundEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = m_BackgroundTile;
	backgroundEntity.GetComponent<TransformComponent>().Scale = { 17.0f, 17.0f, 0.0f };
	backgroundEntity.GetComponent<SpriteRendererComponent>().TilingFactor = 17;

	AnimationBlock wallBreakAnimation;
	{
		wallBreakAnimation.Tag = "WallBreakAnimation";
		wallBreakAnimation.SpriteSize = { 16,16 };
		wallBreakAnimation.SpriteSheet = m_SpriteSheet;

		std::array<glm::vec2, 5> positions;
		std::array<float, 5> delays = { 100.0f, 100.0f, 100.0f, 100.0f, 10.0f };

		positions[0] = { 3,6 };
		positions[1] = { 4,6 };
		positions[2] = { 5,6 };
		positions[3] = { 6,6 };
		positions[4] = { 7,6 };

		wallBreakAnimation.Animation.reserve(positions.size());
		for (size_t i = 0; i < positions.size(); i++)
		{
			FrameData frameData;

			frameData.Subtexture = SubTexture2D::CreateFromCoords(wallBreakAnimation.SpriteSheet, positions[i], wallBreakAnimation.SpriteSize);
			frameData.Delay = delays[i];

			wallBreakAnimation.Animation.emplace_back(frameData);
		}
	}

	// Level Editor
	
	m_MapSkeleton += "#################"; // 17 x 17
	m_MapSkeleton += "#------BBB------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#----BBBB-------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#--------B------#";
	m_MapSkeleton += "#-#-#-#-#-#B#-#-#";
	m_MapSkeleton += "#---BBBBB--BB---#";
	m_MapSkeleton += "#-#-#-#-#-#B#-#-#";
	m_MapSkeleton += "#------BBBB-----#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------M-----#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#------BBBBBB---#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#P--------------#";
	m_MapSkeleton += "#################";
	
	/*
	m_MapSkeleton += "#################"; // 17 x 17
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#B#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#B#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#B#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#B#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#B#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#B#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#BBBBBBBBBBBBBBB#";
	m_MapSkeleton += "#-#B#B#B#B#B#B#B#";
	m_MapSkeleton += "#P-BBBBBBBBBBBBB#";
	m_MapSkeleton += "#################";
	*/
	m_GameGrid = new Entity * [columns];

	for (uint32_t i = 0; i < columns; i++)
		m_GameGrid[i] = new Entity[rows];

	uint32_t i = 0;
	for (int32_t y = 0; y < columns; y++)
	{
		for (int32_t x = 0; x < rows; x++)
		{
			Entity gridEntity = scene->CreateEntityWithDrawOrder(1, "Empty");
			gridEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
			gridEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
			gridEntity.GetComponent<TransformComponent>().Translation = { (8 - x) * (-1), 8 - y, 0.0f };

			gridEntity.AddComponent<Rigidbody2DComponent>();
			gridEntity.AddComponent<BoxCollider2DComponent>();

			m_GameGrid[y][x] = { gridEntity };

			switch (m_MapSkeleton[i])
			{
			case '#': // WALL
				gridEntity.GetComponent<TagComponent>().Tag = "Wall";
				gridEntity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = m_WallTile;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_WALL;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				break;
			case 'B': // LOOT BOX 
				gridEntity.GetComponent<TagComponent>().Tag = "Box";
				gridEntity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
				gridEntity.GetComponent<SpriteRendererComponent>().SubTexture = m_BoxTile;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_BOX;
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = false;
				gridEntity.AddCustomComponent<FrameAnimator>(gridEntity).Add(wallBreakAnimation);
				break;
			case 'P': // PLAYER SPAWN POINT
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_PSP;
				break;
			case 'M': // MONSTER SPAWN POINT
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_MSP;
				break;
			case '-': // EMPTY
				gridEntity.GetComponent<BoxCollider2DComponent>().IsSensor = true;
				break;
			default:
				HU_CORE_ASSERT(false, "Invalid tile!");
				break;
			}
			i++;
		}
	}
}

// Iterates through the whole game grid
// If lambda returns true the search will stop
void GameGrid::ForEach(const std::function<bool(uint32_t, uint32_t, Entity)>& func)
{
	for (uint32_t y = 0; y < m_Columns; y++)
	{
		for (uint32_t x = 0; x < m_Rows; x++)
		{
			if (func(x, y, m_GameGrid[y][x]))
				return;
		}
	}
}

void GameGrid::ForEach_1(const std::function<bool(Entity)>& func)
{
	for (uint32_t y = 0; y < m_Columns; y++)
	{
		for (uint32_t x = 0; x < m_Rows; x++)
		{
			if (func(m_GameGrid[y][x]))
				return;
		}
	}
}
void GameGrid::OnUpdate(Timestep ts)
{
	// TODO: think where this should take place
	for(auto& placed_bomb : g_Player->m_PlacedBombs)
	{
		for (size_t i = 0; i < placed_bomb.m_Destroyed.size(); i++)
		{
			auto& fa = placed_bomb.m_Destroyed[i].GetComponent<FrameAnimator>();
			if (fa.IsAnyPlaying())
			{
				if (!fa.OnUpdate(ts))
				{
					DestroyGridEntity(placed_bomb.m_Destroyed[i]);
					placed_bomb.m_Destroyed.erase(placed_bomb.m_Destroyed.begin() + i);
				}
			}
		}
	}
}

void GameGrid::DestroyGridEntity(Entity entity)
{
	entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
	entity.GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
	entity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;

	if (entity.HasComponent<Rigidbody2DComponent>())
	{
		g_GameScene->DestroyBody(entity);
		entity.RemoveComponent<Rigidbody2DComponent>();

		if (entity.HasComponent<BoxCollider2DComponent>())
			entity.RemoveComponent<BoxCollider2DComponent>();

		entity.AddComponent<Rigidbody2DComponent>().Gravity = false;
		entity.AddComponent<BoxCollider2DComponent>().IsSensor = true;

		g_GameScene->CreateBody(entity);
	}
}

