#pragma once

#include <Hurikan.h>
using namespace Hurikan;

struct GridEntities
{
	Entity GridBackground;
	std::array<Entity, 17 * 17> Blocks;
};

class GameGrid
{
public:
	GameGrid() = default;
	GameGrid(uint32_t rows, uint32_t columns);
	~GameGrid() = default;

	void Init(Ref<Scene> scene);
	void Update(Timestep ts);

	GridEntities& GetGridEntities() { return m_SceneEntities; }

	std::vector<Entity> GetEmptyEntities();
private:
	Ref<Scene> m_Scene;

	GridEntities m_SceneEntities;

	Ref<Texture2D> m_SpriteSheet;
	Ref<Texture2D> m_WallTile;
	Ref<Texture2D> m_BackgroundTile;
	Ref<Texture2D> m_BoxTile;

	Ref<Texture2D> m_Bomb;

	uint32_t m_Rows, m_Columns;

	std::string m_MapSkeleton;
};