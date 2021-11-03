#pragma once

#include <Hurikan.h>
using namespace Hurikan;

// TODO: Organize
#include "../scripts/PlayerAnimationComponent.h"

enum class EntityType
{
	CAMERA, PLAYER, BACKGROUND,
	BOMB, BOMB_SPREAD_EXPLOSION,
	TILE_EMPTY, TILE_WALL, TILE_BOX, TILE_PSP // PlayerStartupPosition 
};

struct EntityTypeComponent
{
	EntityType Type;

	EntityTypeComponent() = default;
	EntityTypeComponent(const EntityTypeComponent&) = default;
	EntityTypeComponent(const EntityType& type) : Type(type) {}
};