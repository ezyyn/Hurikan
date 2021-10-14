#include "GameGrid.h"

GameGrid::GameGrid(uint32_t rows, uint32_t columns) : m_Rows(rows), m_Columns(columns)
{
}

void GameGrid::Init(Ref<Scene> scene)
{
	m_Scene = scene;

	m_BackgroundTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/background.png");
	m_SpriteSheet = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png");
	m_WallTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/wall.png");

	m_SceneEntities.GridBackground = m_Scene->CreateEntity("GameGrid Entity");
	m_SceneEntities.GridBackground.AddComponent<SpriteRendererComponent>().Texture = m_BackgroundTile;
	m_SceneEntities.GridBackground.GetComponent<SpriteRendererComponent>().TilingFactor = 18;
	m_SceneEntities.GridBackground.GetComponent<TransformComponent>().Scale = {17, 17, 1};
/*
	m_SceneEntities.Blocks = new Entity * [m_Rows];
	for (uint32_t i = 0; i < m_Columns; i++)
		m_SceneEntities.Blocks[i] = new Entity[m_Columns];
*/

	m_GameGrid = new char* [m_Rows];
	for (uint32_t i = 0; i < m_Columns; i++)
		m_GameGrid[i] = new char[m_Columns];
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
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#-#-#-#-#-#-#-#-#";
	m_MapSkeleton += "#---------------#";
	m_MapSkeleton += "#################";
	*/
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
		if (m_MapSkeleton[i] == '#')
		{
			m_SceneEntities.Blocks[i] = m_Scene->CreateEntity(std::to_string(i + 1));
			m_SceneEntities.Blocks[i].AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).Texture = m_WallTile;
			m_SceneEntities.Blocks[i].GetComponent<TransformComponent>().Translation = { x, (y * -1), 0.1f };
			HU_INFO("{0} {1}", x, y);
		}
		else {
			/*
			if (i == m_SceneEntities.Blocks.size() - 1 || i == 0)
			{
				m_SceneEntities.Blocks[i] = m_Scene->CreateEntity(std::to_string(i + 1));
				m_SceneEntities.Blocks[i].AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				m_SceneEntities.Blocks[i].GetComponent<TransformComponent>().Translation = { x, y, 0.1f };
			}
			else {
			*/
			m_SceneEntities.Blocks[i] = m_Scene->CreateEntity(std::to_string(i + 1));
			//m_SceneEntities.Blocks[i].AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
			m_SceneEntities.Blocks[i].GetComponent<TransformComponent>().Translation = { x, (y * -1), 0.1f };
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
float ax = 8, ay = 8;

void GameGrid::Update(Hurikan::Timestep& ts)
{/*
	Renderer2D::DrawQuad({ ax,ay,-0.1 }, { 17,17 }, m_BackgroundTile, 18);
	//Renderer2D::DrawQuad({ 0,0, 0.1 }, { 1,1 }, m_WallTile);
	for (uint32_t y = 0; y < m_Rows; y++)
	{
		for (uint32_t x = 0; x < m_Columns; x++)
		{
			switch (m_GameGrid[y][x])
			{
			case '#':
				Renderer2D::DrawQuad({ x,m_Rows - y - 1,0.1 }, { 1,1 }, m_WallTile);
				break;
			}
		}
	}*/
}
