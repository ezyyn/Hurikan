#pragma once

#include "../core/Level.h"

#include "../sceneobjects/Enemy.h"

struct AnimationDetails;

struct GridNode
{
	// SearchAlg stuff
	// TOTALLY ENEMY DEPENDENT
	bool Visited = false;
	float GlobalGoal = 0.0f;
	float LocalGoal = 0.0f;

	// NOT ENEMY DEPENDENT
	bool Obstacle = false;
	
	int IndexX, IndexY;

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
	GridNode* SearchFor(const glm::vec3& position);

	inline const int GetLevelWidth()  const noexcept { return m_CurrentLevel.Width; }
	inline const int GetLevelHeight() const noexcept { return m_CurrentLevel.Height; }

	inline GridNode* WherePlayerAt() noexcept { return m_PlayerGridPosition; }

	//inline GridNode** Get() { return m_GameGrid; }
	inline GridNode* Get(int y, int x) { return &m_GameGrid[y * GetLevelWidth() + x]; }

	inline void BombChanged() { m_BombEvent = true; };
private:
	Level m_CurrentLevel;

	// Sprites
	Ref<Texture2D> m_BackgroundTile;
	Ref<SubTexture2D> m_WallTile;
	Ref<SubTexture2D> m_BoxTile;

	std::vector<GridNode*> m_Destroyable = {};

	GridNode* m_GameGrid = nullptr;

	GridNode* m_PlayerGridPosition = nullptr;
	GridNode* m_PlayerPreviousPosition = nullptr;

	// Monsters
	EnemySpawner m_EnemySpawner;

	// TODO: maybe make an event system ? No
	bool m_BombEvent = false;
};
