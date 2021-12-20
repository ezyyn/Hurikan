#pragma once

#include "../core/Level.h"
#include "../core/AnimationBlock.h"

#include "../sceneobjects/Enemy.h"

#include <Hurikan.h>
using namespace Hurikan;

struct GridNode
{
	// SearchAlg stuff
	bool Obstacle = false;
	bool Visited = false;
	float GlobalGoal;
	float LocalGoal;

	int IndexX = -1, IndexY = -1;

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

	inline int GetLevelWidth() const { return m_CurrentLevel.Width; }
	inline int GetLevelHeight() const { return m_CurrentLevel.Height; }

	GridNode* WherePlayerAt() { return m_PlayerGridPosition; }

	GridNode** Get() { return m_GameGrid; }
	GridNode* Get(int y, int x) { return &m_GameGrid[y][x]; }
private:
	Level m_CurrentLevel;

	// Sprites
	Ref<Texture2D> m_BackgroundTile;
	Ref<SubTexture2D> m_WallTile;
	Ref<SubTexture2D> m_BoxTile;

	AnimationBlock m_WallBreakAnimation;
	std::vector<GridNode*> m_Destroyable;

	GridNode** m_GameGrid = nullptr;

	GridNode* m_PlayerGridPosition;

	// Monsters

};
/*


// TODO: path finding
struct GridNode
{
	bool Obstacle = false;
	bool Visited = false;
	float GlobalGoal;
	float LocalGoal;
	glm::vec2 Position;
	std::vector<GridNode*> Neighbours;

	GridNode* Parent = nullptr;
	Entity Handle;
};

class GameGrid
{
public:
	GameGrid() = default;
	~GameGrid();
	void Init(Scene* scene, Player* player, Level level);

	void ForEach(const std::function<bool(uint32_t, uint32_t, Entity)>& func);
	void ForEach(const std::function<bool(Entity)>& func);

	void OnUpdate(Timestep ts);

	void DestroyGridEntity(Entity entity);

	GridNode** GetGrid() const { return m_GameGrid; }
public:
	int32_t GetLevelWidth() { return m_CurrentLevel.Width; }
	int32_t GetLevelHeight() { return m_CurrentLevel.Height; }
private:
	Scene* g_GameScene;
	Player* g_Player;
private:
	void SolvePath(); // TODO: move this to separate file
private:
	Ref<Texture2D> m_SpriteSheet;

	GridNode** m_GameGrid;

	GridNode* m_Start; // Changes location according to the enemy
	GridNode* m_End; // Changes location according to the player
	GridNode* m_PrevisousEnd = nullptr;
	std::deque<GridNode*> m_Path;
	std::deque<GridNode*> m_Steer;

	int m_CurrentIndex = 0;
	int m_NextIndex = 1;

	Level m_CurrentLevel;

	EnemySpawner m_EnemySpawner;

	friend class Player;
	friend class Bomb;
};
*/