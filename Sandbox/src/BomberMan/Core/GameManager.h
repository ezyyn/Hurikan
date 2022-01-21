#pragma once

#include "BomberMan/Game/InGame.h"

#include <Hurikan/Core/Layer.h>
using namespace Hurikan;

class GameManager : public Layer
{
public:
	GameManager() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep& ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

private:
	InGame m_InGameScene;
	/*
	
	Game
	MainMenu
	
	*/

};