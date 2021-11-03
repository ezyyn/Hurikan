#pragma once

#include <Hurikan.h>
using namespace Hurikan;

// Specific to the game
enum class GameObjectType
{
	CAMERA, PLAYER, BACKGROUND, 
	BOMB, BOMB_SPREAD_EXPLOSION, 
	TILE_EMPTY, TILE_WALL, TILE_BOX
};

class GameObject
{
public:
	GameObject() = default;
	GameObject(Entity entity, GameObjectType type);
	GameObject(const GameObject& other) = default;
private:
	Entity m_GameEntity;
	GameObjectType m_GameObjectType;
};