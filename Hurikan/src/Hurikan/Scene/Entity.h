#pragma once
#include "Scene.h"

#include "Hurikan/Core/Base.h"

#include "entt.hpp"

#include "Hurikan/Core/UUID.h"
#include "Hurikan/Scene/Components.h"

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
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasNativeScript()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			HU_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this,component);
			return component;
		}
		// Removed OnComponentAdded event
		template<typename T, typename... Args>
		T& AddCustomComponent(Args&&... args)
		{
			HU_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			HU_CORE_ASSERT(HasComponent<T>(), "Entity does not have component! {0}");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// Experimental, promising
		template<typename T>
		T* GetNativeScript()
		{
			HU_CORE_ASSERT(HasComponent<NativeScriptComponent>(), "Entity does not have NSC! {0}");
			if (m_Scene->m_Registry.get<NativeScriptComponent>(m_EntityHandle).Instance == nullptr)
				m_Scene->InstantiateScript(Entity(m_EntityHandle, m_Scene));

			return (T*)m_Scene->m_Registry.get<NativeScriptComponent>(m_EntityHandle).Instance;
		}

		template<typename T>
		void RemoveComponent()
		{
			HU_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		
		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return *this == other; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}