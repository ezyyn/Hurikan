#pragma once

#include "BomberMan/Core/Observer.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

using namespace Hurikan;

class GCamera : public Observer, public Observable
{
public:
	void Create(Scene* scene);


	void OnUpdate(Timestep& ts);

private:
	void OnGameEvent(GameEvent& e) override;
private:
	Entity m_PlayerCamera;
	Entity m_CutSceneCamera;

	glm::vec3 m_PlayerPosition;
	Entity m_BossEntity;

	Entity m_BlackSquare;
	
	// Boss fight scene
	bool m_BossFightZoom = false;
	bool m_BossFight = false;

	bool m_GameOver = false;

	Scene* g_GameScene;
};