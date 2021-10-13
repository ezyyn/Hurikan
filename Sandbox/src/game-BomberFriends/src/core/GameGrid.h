#pragma once

#include <Hurikan.h>
using namespace Hurikan;

class GameGrid
{
public:
	GameGrid(uint32_t width, uint32_t height);
	~GameGrid() = default;

	void Init();
	void Update(Hurikan::Timestep& ts);
private:
	Ref<Texture2D> m_SpriteSheet;
	Ref<SubTexture2D> m_WallTile;
	Ref<Texture2D> m_BackgroundTile;

	uint32_t m_Width, m_Height;

	std::string m_MapSkeleton;

	char** m_GameGrid = nullptr;
};