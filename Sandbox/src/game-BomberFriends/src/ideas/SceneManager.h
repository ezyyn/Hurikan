#pragma once

#include <Hurikan.h>
using namespace Hurikan;

#include "GameObject.h"

class SceneManager 
{
public:
	SceneManager() = default;

	void Instantiate();

	GameObject CreateGameObject(Entity entity, GameObjectType type);

	void Update(Timestep ts);
private:
	Ref<Scene> m_Scene;
};