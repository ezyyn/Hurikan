#include "GameGrid.h"

GameGrid::GameGrid(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
{
	Init();
}

void GameGrid::Init()
{
	m_GameGrid = new char* [m_Height];
	for (uint32_t i = 0; i < m_Width; i++)
		m_GameGrid[i] = new char[m_Width];

	m_SpriteSheet = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png");

	m_WallTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8,9 }, { 64,64 });
	m_BackgroundTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/background.png");

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


	for (size_t y = 0; y < m_Height; y++)
	{
		for (size_t x = 0; x < m_Width; x++)
		{
			m_GameGrid[y][x] = m_MapSkeleton[x + y * m_Height];
		}
	}
}
float ax = 8, ay = 8;

void GameGrid::Update(Hurikan::Timestep& ts)
{
	Renderer2D::DrawQuad({ ax,ay,-0.1 }, { 17,17 }, m_BackgroundTile, 18);
	//Renderer2D::DrawQuad({ 0,0, 0.1 }, { 1,1 }, m_WallTile);
	for (uint32_t y = 0; y < m_Height; y++)
	{
		for (uint32_t x = 0; x < m_Width; x++)
		{
			switch (m_GameGrid[y][x])
			{
			case '#':
				Renderer2D::DrawQuad({ x,m_Height - y - 1,0.1 }, { 1,1 }, m_WallTile);
				break;
			}
		}
	}
}
