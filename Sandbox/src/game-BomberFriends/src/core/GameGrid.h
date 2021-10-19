#pragma once

#include <Hurikan.h>
using namespace Hurikan;

struct SceneEntities
{
	Entity GridBackground;
	std::array<Entity, 17 * 17> Blocks;
};

class GameGrid
{
public:
	GameGrid(uint32_t rows, uint32_t columns);
	~GameGrid() = default;

	void Init(Ref<Scene> scene);
	void Update(Hurikan::Timestep& ts);

	SceneEntities& GetSceneEntities() { return m_SceneEntities; }
private:
	Ref<Scene> m_Scene;

	SceneEntities m_SceneEntities;

	Ref<Texture2D> m_SpriteSheet;
	Ref<Texture2D> m_WallTile;
	Ref<Texture2D> m_BackgroundTile;

	Ref<Texture2D> m_Bomb;

	uint32_t m_Rows, m_Columns;

	std::string m_MapSkeleton;

	char** m_GameGrid = nullptr;
};