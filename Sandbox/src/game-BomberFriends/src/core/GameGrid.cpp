#include "GameGrid.h"

#include "TextureLoader.h"

GameGrid::GameGrid(uint32_t rows, uint32_t columns) : m_Rows(rows), m_Columns(columns)
{
}

void GameGrid::Init(Ref<Scene> scene)
{
	m_Scene = scene;

	m_BackgroundTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/background.png");
	//m_SpriteSheet = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png");
	m_WallTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/wall.png");

	m_SceneEntities.GridBackground = m_Scene->CreateEntity("GameGridEntity");
	m_SceneEntities.GridBackground.AddComponent<SpriteRendererComponent>().Texture = m_BackgroundTile;
	m_SceneEntities.GridBackground.GetComponent<SpriteRendererComponent>().TilingFactor = 18.0f;
	m_SceneEntities.GridBackground.GetComponent<TransformComponent>().Scale = { 17, 17, 1 };
	m_SceneEntities.GridBackground.GetComponent<TransformComponent>().Translation.z = 1.0f;

	m_GameGrid = new char* [m_Rows];
	for (uint32_t i = 0; i < m_Columns; i++)
		m_GameGrid[i] = new char[m_Columns];
	
	m_MapSkeleton += "#################";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#------B--------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#P--------------#";
	m_MapSkeleton += "#################";
	
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
	for (size_t y = 0; y < m_Rows; y++)
	{
		for (size_t x = 0; x < m_Columns; x++)
		{
			m_GameGrid[y][x] = m_MapSkeleton[x + y * m_Rows];
		}
	}

	float x = -8, y = -8;
	
	for (size_t i = 0; i < m_SceneEntities.Blocks.size(); i++)
	{
		m_SceneEntities.Blocks[i] = m_Scene->CreateEntity();

		auto& entity_block = m_SceneEntities.Blocks[i];
		entity_block.GetComponent<TransformComponent>().Translation = { x, (y * -1), 0.0f };
		auto& rb =entity_block.AddComponent<Rigidbody2DComponent>();
		rb.Type = Rigidbody2DComponent::BodyType::Static;

		switch (m_MapSkeleton[i])
		{
			case '#':
			{
				entity_block.GetComponent<TagComponent>().Tag = "Wall";
				entity_block.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).Texture = m_WallTile;
				entity_block.AddComponent<BoxCollider2DComponent>();
				break;
			}
			default:
			{
				entity_block.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
				if (m_MapSkeleton[i] == 'P') // marking startup position for 
				{
					entity_block.GetComponent<TagComponent>().Tag = "PlayerStartingPositionEntity";
					break;
				}
				entity_block.GetComponent<TagComponent>().Tag = "Empty";
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

void GameGrid::Update(Hurikan::Timestep& ts)
{
}
