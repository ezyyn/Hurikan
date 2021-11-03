#include "GameGrid.h"

#include "../core/TextureLoader.h"
#include "../core/CustomComponents.h"

GameGrid::GameGrid(uint32_t rows, uint32_t columns) : m_Rows(rows), m_Columns(columns)
{
}

void GameGrid::Init(Ref<Scene> scene)
{
	m_Scene = scene;

	m_BackgroundTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/background.png");
	m_SpriteSheet = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png");
	m_WallTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/wall.png");
	m_BoxTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/box.png");

	m_SceneEntities.GridBackground = m_Scene->CreateEntityWithDrawOrder(0,"GameGridEntity"); 

	m_SceneEntities.GridBackground.AddCustomComponent<EntityTypeComponent>(EntityType::BACKGROUND);

	auto& src = m_SceneEntities.GridBackground.AddComponent<SpriteRendererComponent>();
	src.Texture = m_BackgroundTile;
	src.TilingFactor = 18.0f;

	m_SceneEntities.GridBackground.GetComponent<TransformComponent>().Scale = { 17, 17, 1 };

	// Level Editor
	m_MapSkeleton += "#################";
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

	/*
	m_MapSkeleton += "#################";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#--------B------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#P--------------#";
	m_MapSkeleton += "#################";
	*/
	
	/*
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "-----------------";
	m_MapSkeleton += "--------#--------";
	m_MapSkeleton += "-----------------";
	*/

	// Grid Creation
	float x = -8, y = -8;
	
	for (size_t i = 0; i < m_SceneEntities.Blocks.size(); i++)
	{
		auto& entity_block = m_SceneEntities.Blocks[i];

		entity_block = m_Scene->CreateEntityWithDrawOrder(1, "Empty");
		entity_block.GetComponent<TransformComponent>().Translation = { x, (y * -1), 0.0f };
		switch (m_MapSkeleton[i])
		{
			case '#':
			{
				entity_block.GetComponent<TagComponent>().Tag = "Wall";
				entity_block.AddCustomComponent<EntityTypeComponent>(EntityType::TILE_WALL);
				entity_block.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).Texture = m_WallTile;
				entity_block.AddComponent<BoxCollider2DComponent>();

				auto& rb = entity_block.AddComponent<Rigidbody2DComponent>();
				rb.Type = Rigidbody2DComponent::BodyType::Static;
				break;
			}
			case 'B':
			{
				entity_block.GetComponent<TagComponent>().Tag = "Box";
				entity_block.AddCustomComponent<EntityTypeComponent>(EntityType::TILE_BOX);
				entity_block.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).Texture = m_BoxTile;
				entity_block.AddComponent<BoxCollider2DComponent>();

				auto& rb = entity_block.AddComponent<Rigidbody2DComponent>();
				rb.Type = Rigidbody2DComponent::BodyType::Static;
				break;
			}
			default:
			{
				entity_block.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
				entity_block.AddCustomComponent<EntityTypeComponent>(EntityType::TILE_EMPTY);
				if (m_MapSkeleton[i] == 'P') // Marking startup position for player
				{
					entity_block.GetComponent<TagComponent>().Tag = "PlayerStartingPositionEntity";
					entity_block.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_PSP;
				}
				break;
			}
		}

		if (x == -8 + m_Rows - 1)
		{
			x = -8;
			y++;
		}
		else
		{
			x++;
		}
	}
}

void GameGrid::Update(Hurikan::Timestep ts)
{
}

std::vector<Hurikan::Entity> GameGrid::GetEmptyEntities()
{
	std::vector<Entity> empty_entities;

	for (auto& block : m_SceneEntities.Blocks)
	{
		if (block.GetComponent<EntityTypeComponent>().Type == EntityType::TILE_EMPTY)
		{
			empty_entities.push_back(block);
		}
	}

	return empty_entities;
}
