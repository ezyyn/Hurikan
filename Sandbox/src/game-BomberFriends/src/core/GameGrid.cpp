#include "GameGrid.h"

GameGrid::GameGrid(unsigned int width, unsigned int height) : m_Width(width), m_Height(height)
{
}

void GameGrid::Init()
{
	m_GameGrid = new char*[m_Height];
	for (uint32_t i = 0; i < m_Width; i++)
		m_GameGrid[i] = new char[m_Width];

	m_SpriteSheet = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/tileset.png");
	m_SubTextureList[0] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,1 }, { 128,128 });

	m_TextureMap['#'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,1 }, { 128,128 });

	m_MapSkeleton += "#################";
	m_MapSkeleton += "#--------------O#";
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

	for (size_t y = 0; y < m_Height; y++)
	{
		for (size_t x = 0; x < m_Width; x++)
		{
			m_GameGrid[y][x] = m_MapSkeleton[x + y * m_Height];
		}
	}


	/*
	for (size_t y = 0; y < m_Height; y++)
	{
		for (size_t x = 0; x < m_Width; x++)
		{
			std::cout << m_GameGrid[y][x];
		}
		std::cout << std::endl;
	}*/
}

void GameGrid::Draw()
{
	//Renderer2D::DrawQuad({ 1,1,0 }, { 1,1 }, m_SubTextureList[0]);
}

void GameGrid::Update(Hurikan::Timestep ts)
{
}
