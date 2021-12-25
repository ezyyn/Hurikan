#pragma once

#include "../core/Level.h"
#include "../core/AnimationBlock.h"

#include "../sceneobjects/Enemy.h"

struct GridNode
{
	// SearchAlg stuff
	bool Obstacle = false;
	bool Visited = false;
	float GlobalGoal;
	float LocalGoal;

	glm::vec2 Position = {};

	std::vector<GridNode*> Neighbours;
	GridNode* Parent = nullptr;

	Entity Handle;
};

enum class EntityType
{
	CAMERA, PLAYER, BACKGROUND,
	BOMB, BOMB_SPREAD_EXPLOSION,
	TILE_EMPTY, TILE_TO_BE_EMPTY,
	TILE_WALL, TILE_BOX, TILE_PSP, TILE_MSP,
	ENEMY
};

struct EntityTypeComponent
{
	EntityType Type;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(const EntityType& type) : Type(type) {}
};

class Grid
{
public:
	Grid() = default;
	~Grid() = default;

	void Init();
	void OnUpdate(Timestep ts);
	void Shutdown();

	void Generate(const Level& level, glm::vec2* const startpos);

	void Each(const std::function<bool(GridNode*)>& func);
	GridNode* SearchFor(Entity entity);
	GridNode* SearchFor(glm::vec3 position);

	inline int GetLevelWidth()  const noexcept { return m_CurrentLevel.Width; }
	inline int GetLevelHeight() const noexcept { return m_CurrentLevel.Height; }

	inline GridNode* WherePlayerAt() noexcept { return m_PlayerGridPosition; }

	inline GridNode** Get() { return m_GameGrid; }
	inline GridNode* Get(int y, int x) { return &m_GameGrid[y][x]; }

	inline void BombChanged() { m_BombEvent = true; };
private:
	Level m_CurrentLevel;

	// Sprites
	Ref<Texture2D> m_BackgroundTile;
	Ref<SubTexture2D> m_WallTile;
	Ref<SubTexture2D> m_BoxTile;

	AnimationBlock m_WallBreakAnimation;
	std::vector<GridNode*> m_Destroyable;

	GridNode** m_GameGrid = nullptr;

	GridNode* m_PlayerGridPosition = nullptr;
	GridNode* m_PlayerPreviousPosition = nullptr;

	// Monsters
	EnemySpawner m_EnemySpawner;

	// TODO: maybe make an event system ?
	bool m_BombEvent = false;
};
