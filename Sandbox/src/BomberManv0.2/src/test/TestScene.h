#pragma once

#include <Hurikan.h>
using namespace Hurikan;

class TestScene
{
public:
	TestScene() = default;
	~TestScene() = default;

	void Init();
	void OnUpdate(Timestep ts);
	void Shutdown();

	//void OnKeyPressed(KeyPressedEvent& e);
	//void OnKeyReleased(KeyReleasedEvent& e);
private:
	Entity m_Camera;
	TransformComponent* m_CameraTransform = nullptr;

	Ref<Scene> m_TestScene;
	//BombManager m_BombManager;
	//
	//Level m_CurrentLevel;

	/*
	GameGrid m_GameGrid;
	Player m_Player;
	CollissionDetector m_CollisionDetector;
	GameCamera m_GameCamera;
	*/
};