#pragma once

#include "../sceneobjects/Bomb.h"
#include "../ui/SimpleUI.h"

#include <Hurikan.h>
using namespace Hurikan;

class Game
{
public:

	struct CameraBoundaries
	{
		float MinX = 0.0f;
		float MinY = 0.0f;
		float MaxX = 0.0f;
		float MaxY = 0.0f;
	};


	Game() = default;
	~Game() = default;

	void Init();
	void Play(const Level& level);
	void OnUpdate(Timestep ts);
	void Shutdown();
private:
	KeyCode m_LastKeyPressed = 0;

	Entity m_Camera;
	CameraBoundaries m_Boundaries;
	TransformComponent* m_CameraTransform = nullptr;

	SimpleUI m_UI;

	BombManager m_BombManager;

	Level m_CurrentLevel;
};