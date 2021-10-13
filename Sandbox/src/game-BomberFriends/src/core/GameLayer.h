#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "GameGrid.h"

#include "Hurikan/Renderer/OrthographicCameraController.h"

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
private:
	OrthographicCameraController m_CameraController;
	GameGrid m_GameGrid;
};