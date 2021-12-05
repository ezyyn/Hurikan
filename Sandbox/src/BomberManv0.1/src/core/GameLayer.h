#pragma once

#include <Hurikan.h>
using namespace Hurikan;

// maybe move to core
#include "../menu/GameMenu.h"

#include "../core/GameApplication.h"

class GameLayer : public Layer
{
public:
	GameLayer() = default;
	GameLayer(GameApplication* app);
	~GameLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep& ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;

	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	GameApplication* g_App;
private:
	GameMenu m_GameMenu;
};