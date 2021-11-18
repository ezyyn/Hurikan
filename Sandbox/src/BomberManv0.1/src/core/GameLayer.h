#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "../sceneobjects/GameCamera.h"
#include "../sceneobjects/GameGrid.h"
#include "../sceneobjects/Player.h"

#include "CollisionDetector.h"

class GameLayer : public Layer
{
public:
	GameLayer(uint32_t width, uint32_t height);

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep& ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	Scene m_InGameScene;

	GameGrid m_GameGrid;
	Player m_Player;
	CollissionDetector m_CollisionDetector;
	GameCamera m_GameCamera;

	uint32_t m_Width;
	uint32_t m_Height;
};