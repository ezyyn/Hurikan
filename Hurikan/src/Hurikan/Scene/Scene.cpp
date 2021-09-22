#include "hupch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "Hurikan/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hurikan
{

	static void OnTransformConstruct(entt::registry& _registry, entt::entity _entity)
	{

	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (m_Registry.all_of<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto view = m_Registry.view<TransformComponent>();

		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);

		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);

		//	Renderer::Submit(mesh, transform);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep& ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.InstantiateFunction();
						nsc.Instance->m_Entity = Entity{ entity, this };

						if (nsc.OnCreateFunction)
							nsc.OnCreateFunction(nsc.Instance);
					}

					if (nsc.OnUpdateFunction)
						nsc.OnUpdateFunction(nsc.Instance, ts);
				});
		}

		// Remder 2D
		Camera* main_camera = nullptr;
		glm::mat4* camera_transform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for(auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					main_camera = &camera.Camera;
					camera_transform = &transform.Transform;
					break;
				}
			}
		}

		if (main_camera)
		{
			Renderer2D::BeginScene(main_camera->GetProjection(), *camera_transform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera_component = view.get<CameraComponent>(entity);
			if (!camera_component.FixedAspectRatio)
			{
				camera_component.Camera.SetViewportSize(width, height);
			}
		}
	}

}