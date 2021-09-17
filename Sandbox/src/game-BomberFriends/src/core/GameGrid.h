#pragma once
#include "Entity.h"

class GameGrid : public Entity
{
public:
	GameGrid(unsigned int width, unsigned int height);
	~GameGrid() = default;

	void Init();
	void Draw() override;
	void Update(Timestep ts) override;
private:
	unsigned int m_Width, m_Height;

	std::string m_MapSkeleton;
	std::unordered_map<char, Ref<Texture2D>> m_TextureMap;
	Hurikan::Ref<Hurikan::Texture2D> m_SpriteSheet;
	std::array<Ref<SubTexture2D>, 64> m_SubTextureList;

	char** m_GameGrid;

	//Level map
};

