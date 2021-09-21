#include "hupch.h"
#include "Entity.h"

namespace Hurikan
{
	Entity::Entity(entt::entity id, Scene* scene) : m_EntityID(id), m_Scene(scene)
	{
	}
}