#pragma once
#include "GameEntity.h"

enum BoxTypeEnum
{
	WALL,BOX,EMPTY
};

struct BoxProperties
{
	uint32_t X, Y;
	uint32_t Width, Height;
	BoxTypeEnum BoxType;
	//Ref<Texture2D> Texture;
	/*BoxProperties(float _x, float _y, float _width, float _height, Ref<Texture2D> _texture ) : X(_x), Y(_y), Width(_width), Height(_height), Texture(_texture)
	{
	}*/
};
class GameGrid : public GameEntity
{
public:
	GameGrid(unsigned int width, unsigned int height);
	~GameGrid() = default;

	BoxProperties** GetBoxProperties() { return m_CollisionGrid; }

	void Init();
	void Draw();
	void Update(Timestep& ts);
private:
	unsigned int m_Width, m_Height;

	std::string m_MapSkeleton;
	
	Ref<Texture2D> m_SpriteSheet;

	Ref<SubTexture2D> m_WallTile;
	Ref<Texture2D> m_BackgroundTile;

	char** m_GameGrid = nullptr;

	BoxProperties** m_CollisionGrid;

	//Level map
};

