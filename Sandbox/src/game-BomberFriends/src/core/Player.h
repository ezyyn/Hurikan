#pragma once
#include "GameEntity.h"
#include "GameGrid.h"

enum PlayerEffectType
{
	SPEED, MAXBOMB, CURSE/*time-bomb explosion random*/, FEAR
};

struct PlayerEffect
{
	uint32_t Speed = 2;
	uint32_t MaxBomb = 1;
	bool Shield = false;
	bool Curse = false;
	bool Fear = false;

	void AddEffect(PlayerEffectType& effect)
	{
		switch (effect)
		{
		case SPEED:
			Speed++;
			break;
		case MAXBOMB:
			MaxBomb++;
			break;
		case CURSE:
			break;
		case FEAR:
			break;
		}
	}
};
struct PlayerStats
{
	uint32_t Health = 1;
	std::string Name = "Default Name";
	uint32_t BombCount = 1;
	PlayerEffect Effect;
};

struct PlayerTransform
{
	glm::vec2 velocity;

	float x, y;
	//float width, height;
	float center_x, center_y;
	float radius;
};

class Player : public GameEntity
{
public:
	Player(OrthographicCameraController& controller);

	void LoadBoxProperties(BoxProperties** boxprops) { m_GridCollision = boxprops; }

	void Draw() override;
	void Update(Timestep& ts) override; 
private:
	OrthographicCameraController m_Controller;
	PlayerStats m_PlayerStats;
	Ref<Texture2D> m_PlayerTexture;

	PlayerTransform m_PlayerBorder;

	BoxProperties** m_GridCollision;
};


