#pragma once
#include "Entity.h"

enum PlayerEffectType
{
	SPEED, MAXBOMB, CURSE/*time-bomb explosion random*/, FEAR
};

struct PlayerEffect
{
	uint32_t Speed = 1;
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

class Player : public Entity
{
public:
	Player(OrthographicCameraController& controller);
	~Player() = default;

	void Draw() override;
	void Update(Timestep ts) override;
private:
	OrthographicCameraController m_Controller;
	PlayerStats m_Stats;
	glm::vec2 m_Position;
};

