#pragma once
#include "entt.hpp"

#include "Hurikan/Core/UUID.h"
#include "Hurikan/Core/Timestep.h"

#include "Hurikan/Renderer/EditorCamera.h"

class b2World;
class b2ContactListener;

namespace Hurikan
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntitywithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity& entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void UpdatePhysics(Timestep ts);

		void OnUpdateEditor(Timestep ts,EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
		
		template<typename... Components>
		auto& GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
		Entity GetEntityByTag(const std::string& _tag);
		Entity GetEntityByUUID(UUID uuid);

		// TODO: Find better way to do this
		void InstantiateScript(Entity entity);

		Entity CreateEntityWithDrawOrder(int order, const std::string & name = std::string());
		void ChangeDrawIndex(int index, Entity entity);

		void CreateBody(Entity& entity);
		void ChangeBody(Entity& entity);
		void DestroyBody(Entity& entity);

		void SetContactListener(b2ContactListener* listener);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	public:
		// TODO: Find better way to do this
		std::vector<std::pair<int, Entity>> m_DrawOrder;
		std::list<Entity> m_DestroyBodiesPool, m_CreateBodiesPool, m_ChangeBodiesPool;
	};
}