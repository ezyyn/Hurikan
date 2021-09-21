#pragma once
#include "Scene.h"

#include "entt.hpp"

namespace Hurikan
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityID);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			HU_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			HU_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			HU_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove(T)(m_EntityID);
		}

		operator bool() const { return m_EntityID != entt::null; }
	private:
		entt::entity m_EntityID = { entt::null };
		Scene* m_Scene = nullptr;
	};
}