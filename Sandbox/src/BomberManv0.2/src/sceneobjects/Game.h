#pragma once

#include "../core/Level.h"

#include "../sceneobjects/Bomb.h"

#include <Hurikan.h>
using namespace Hurikan;

class Game
{
public:
	Game() = default;
	~Game() = default;

	void Init();
	void Play(const Level& level);
	void OnUpdate(Timestep ts);
	void Shutdown();

	void OnKeyPressed(KeyPressedEvent& e);
	void OnKeyReleased(KeyReleasedEvent& e);
private:
	Entity m_Camera;
	TransformComponent* m_CameraTransform = nullptr;

	BombManager m_BombManager;

	Level m_CurrentLevel;

	/*
	GameGrid m_GameGrid;
	Player m_Player;
	CollissionDetector m_CollisionDetector;
	GameCamera m_GameCamera;
	*/
};