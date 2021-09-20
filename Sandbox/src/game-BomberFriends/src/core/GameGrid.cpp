#include "GameGrid.h"

GameGrid::GameGrid(unsigned int width, unsigned int height) : m_Width(width), m_Height(height)
{
}

void GameGrid::Init()
{
	m_GameGrid = new char*[m_Height];
	for (uint32_t i = 0; i < m_Width; i++)
		m_GameGrid[i] = new char[m_Width];

	m_SpriteSheet = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png");

	m_WallTile = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8,9 }, { 64,64 });
	m_BackgroundTile = Hurikan::Texture2D::Create("src/game-BomberFriends/assets/textures/background.png");
	//m_TextureMap['-'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 13,7 }, { 64,64 });
	//m_TextureMap['P'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 5,1 }, { 64,64 });

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

	m_CollisionGrid = new BoxProperties * [m_Height];
	for (uint32_t i = 0; i < m_Width; i++)
		m_CollisionGrid[i] = new BoxProperties[m_Width];

	for (uint32_t y = 0; y < m_Height; y++)
	{
		for (uint32_t x = 0; x < m_Width; x++)
		{
			if (m_MapSkeleton[x + y * m_Height] == '#')
			{
				m_CollisionGrid[y][x].X = x;
				m_CollisionGrid[y][x].Y = m_Height - y;
				m_CollisionGrid[y][x].Width = 1;
				m_CollisionGrid[y][x].Height = 1;
				m_CollisionGrid[y][x].BoxType = BoxTypeEnum::WALL;
			}
			else {
				m_CollisionGrid[y][x].X = x;
				m_CollisionGrid[y][x].Y = m_Height - y;
				m_CollisionGrid[y][x].Width = 1;
				m_CollisionGrid[y][x].Height = 1;
				m_CollisionGrid[y][x].BoxType = BoxTypeEnum::EMPTY;
			}
		}
	}
}

int tiling = 1;

float ax = 8, ay = 8;

void GameGrid::Draw()
{
	Renderer2D::DrawQuad({ ax,ay,-0.1 }, { 17,17 }, m_BackgroundTile,18);
	for (uint32_t y = 0; y < m_Height; y++)
	{
		for (uint32_t x = 0; x < m_Width; x++)
		{
			if (m_CollisionGrid[y][x].BoxType == BoxTypeEnum::WALL)
			Renderer2D::DrawQuad({ m_CollisionGrid[y][x].X,m_CollisionGrid[y][x].Y,0 }, 
				{ m_CollisionGrid[y][x].Width,  m_CollisionGrid[y][x].Height},
				/*m_WallTile*/{0,0,0,1});
		}
	}
}
void GameGrid::Update(Hurikan::Timestep& deltaTime)
{
	//HU_INFO("{0} | {1}", ax, ay);
	/*
	if (Input::IsKeyPressed(Key::Left))
	{
		ax -= deltaTime * 1;
	}
	if (Input::IsKeyPressed(Key::Right)) {
		ax += deltaTime * 1;
	}
	if (Input::IsKeyPressed(Key::Up)) {
		ay += deltaTime * 1;
	}
	if (Input::IsKeyPressed(Key::Down)) {
		ay -= deltaTime * 1;
	}*/

	if (Input::IsKeyPressed(Key::B))
	{
		tiling++;
	}
}
