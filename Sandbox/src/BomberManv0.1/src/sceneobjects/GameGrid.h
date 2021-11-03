#pragma once

#include <Hurikan.h>
using namespace Hurikan;

struct Cell
{
	Cell() = default;
	Cell(Entity cellEntity) : CellEntity(cellEntity)
	{
		X = CellEntity.GetComponent<TransformComponent>().Translation.x;
		Y = CellEntity.GetComponent<TransformComponent>().Translation.y;
	}

	Entity CellEntity;
	float X = 0.0f;
	float Y = 0.0f;
};


class GameGrid
{
public:
	GameGrid() = default;
	void Init(Ref<Scene> scene, uint32_t rows, uint32_t columns);
private:
	Ref<Scene> m_GameScene;
	Ref<Texture2D> m_SpriteSheet;

	Cell** m_GameGrid;
	std::string m_MapSkeleton;

	uint32_t m_Columns;
	uint32_t m_Rows;

	friend class Player;
};