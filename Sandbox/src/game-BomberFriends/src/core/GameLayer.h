#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "GameGrid.h"
#include "Player.h"
#include "GameCamera.h"

class GameLayer : public Layer
{
public:
	GameLayer(uint32_t width, uint32_t height);
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep& ts);
	void OnImGuiRender();
	void OnEvent(Event& e);

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	uint32_t m_Width;
	uint32_t m_Height;

	Ref<Scene> m_Scene;

	// Scene objects
	Player m_Player;
	GameGrid m_GameGrid;
	GameCamera m_GameCamera;
};