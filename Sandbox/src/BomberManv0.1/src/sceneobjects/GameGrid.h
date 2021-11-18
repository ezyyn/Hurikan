#pragma once

enum class EntityType;

#include <Hurikan.h>
using namespace Hurikan;

class GameGrid
{
public:
	GameGrid() = default;
	void Init(Scene* scene, uint32_t rows, uint32_t columns);

	void ForEach(const std::function<bool(uint32_t, uint32_t, Entity)>& func);
	void ForEach_1(const std::function<bool(Entity)>& func);

	void OnUpdate(Timestep ts);

	void DestroyGridEntity(Entity entity);
public:
	uint32_t GetColumns() { return m_Columns; }
	uint32_t GetRows() { return m_Rows; }
private:
	Scene* m_GameScene;
	Ref<Texture2D> m_SpriteSheet;

	Entity** m_GameGrid;
	std::string m_MapSkeleton;

	uint32_t m_Columns;
	uint32_t m_Rows;

	friend class Player;
	friend class Bomb;
};