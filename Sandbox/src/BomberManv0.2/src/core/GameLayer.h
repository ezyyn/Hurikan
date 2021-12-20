#pragma once

#include "../core/SceneManager.h"

#include <Hurikan.h>
using namespace Hurikan;

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep& ts) override;
	void OnImGuiRender() override;
	void OnEvent(Event& e) override;
private:
	SceneManager m_SceneManager;
};