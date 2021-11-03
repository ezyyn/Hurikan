#include "SceneManager.h"

void SceneManager::Instantiate()
{
	m_Scene = CreateRef<Scene>();
}

GameObject SceneManager::CreateGameObject(Entity entity, GameObjectType type)
{
	return {};
}

void SceneManager::Update(Timestep ts)
{

}
