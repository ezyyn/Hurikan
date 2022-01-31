#pragma once

#include "BomberMan/Core/Observer.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

using namespace Hurikan;

class GCamera : public Observer
{
public:
	void Create(Entity& e);

	void OnGameEvent(GameEvent& e) override;

private:
	Entity m_Handle;
	SceneCamera m_ShakyCamera;

	glm::vec3 m_PlayerPosition;
};