#include "GameGrid.h"

#include "../core/GameComponents.h"

void GameGrid::Init(Ref<Scene> scene, uint32_t rows, uint32_t columns)
{
	m_Rows = rows;
	m_Columns = columns;

	m_SpriteSheet = Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png");
	Ref<Texture2D> m_BackgroundTile = Texture2D::Create("src/game-BomberFriends/assets/textures/background.png");
	Ref<SubTexture2D> m_WallTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 1 }, { 64, 64 });
	Ref<SubTexture2D> m_BoxTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 7 }, { 64, 64 });

	Entity backgroundEntity = scene->CreateEntityWithDrawOrder(0, "Gamegrid-Background");
	backgroundEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = m_BackgroundTile;
	backgroundEntity.GetComponent<TransformComponent>().Scale = { 17.0f, 17.0f, 1.0f };
	backgroundEntity.GetComponent<SpriteRendererComponent>().TilingFactor = 17;
	// Level Editor
	
	m_MapSkeleton += "#################"; // 17 x 17
	m_MapSkeleton += "#------BBB------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#--------B------#";
	m_MapSkeleton += "#-#-#-#-#-#B#-#-#";
	m_MapSkeleton += "#----------BB---#";
	m_MapSkeleton += "#-#-#-#-#-#B#-#-#";
	m_MapSkeleton += "#------BBBB-----#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
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
	

	m_GameGrid = new Cell * [columns];

	for (uint32_t i = 0; i < columns; i++)
		m_GameGrid[i] = new Cell[rows];

	uint32_t i = 0;
	for (int32_t y = 0; y < columns; y++)
	{
		for (int32_t x = 0; x < rows; x++)
		{
			Entity cellEntity = scene->CreateEntityWithDrawOrder(1, "Empty");
			cellEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
			cellEntity.AddCustomComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
			cellEntity.GetComponent<TransformComponent>().Translation = { (8 - x) * (-1), 8 - y, 0.0f };
			m_GameGrid[y][x] = { cellEntity };

			switch (m_MapSkeleton[i])
			{
			case '#':
				cellEntity.GetComponent<TagComponent>().Tag = "Wall";
				cellEntity.AddComponent<Rigidbody2DComponent>();
				cellEntity.AddComponent<BoxCollider2DComponent>();
				cellEntity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
				cellEntity.GetComponent<SpriteRendererComponent>().SubTexture = m_WallTile;
				cellEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_WALL;
				break;
			case 'B':
				cellEntity.GetComponent<TagComponent>().Tag = "Box";
				cellEntity.AddComponent<Rigidbody2DComponent>();
				cellEntity.AddComponent<BoxCollider2DComponent>();
				cellEntity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
				cellEntity.GetComponent<SpriteRendererComponent>().SubTexture = m_BoxTile;
				cellEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_BOX;
				break;
			default:
				if (m_MapSkeleton[i] == 'P')
				{
					cellEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_PSP;
				}
				break;
			}
			i++;
		}
	}
}
