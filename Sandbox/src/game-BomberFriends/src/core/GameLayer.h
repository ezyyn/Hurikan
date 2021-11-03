#pragma once

#include "../gameobjects/GameGrid.h"
#include "../gameobjects/Player.h"
#include "../gameobjects/GameCamera.h"

#include <Hurikan.h>
using namespace Hurikan;

class GameLayer : public Layer
{
public:
	GameLayer(uint32_t width, uint32_t height);
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep& ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	uint32_t m_Width;
	uint32_t m_Height;

	Ref<Scene> m_Scene;

	// Scene objects
	Player m_Player;
	Ref<GameGrid> m_GameGrid;
	GameCamera m_GameCamera;
};