#include "hupch.h"
#include "Scene.h"

#include "Components.h"
#include "ScriptableEntity.h"
#include "Entity.h"

#include "Hurikan/Renderer/Renderer2D.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_joint.h"

#include <glm/glm.hpp>

namespace Hurikan
{
	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		HU_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		if(m_PhysicsWorld != nullptr)
			OnRuntimeStop();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntitywithUUID(UUID(), name);
	}
	Entity Scene::CreateEntitywithUUID(UUID uuid, const std::string& name /*= std::string()*/)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
	//	entity.AddComponent<RelationshipComponent>().Parent = entity; // TODO: should apply only on parentless entities
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		// Removes it from draw order
		for (int i = 0; i < m_DrawOrder.size(); i++)
		{
			if (entity == m_DrawOrder[i].second)
			{
				m_DrawOrder.erase(m_DrawOrder.begin() + i);
			}
		}
		// If entity has any children attached to it, remove them as well
		//for (auto& child : entity.GetChildren())
		//{
		//	if (child.HasComponent<Rigidbody2DComponent>())
		//		DestroyBody(child);
		//	m_Registry.destroy(child);
		//}
		// Destroys RigidBody in b2World too
		if (entity.HasComponent<Rigidbody2DComponent>())
			DestroyBody(entity);

		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			CreateBody(entity);
		}
	}

	void Scene::OnRuntimeStop()
	{
		for (int i = 0; i < m_PhysicsUserData.size(); i++)
			delete m_PhysicsUserData[i];

		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::UpdatePhysics(Timestep ts)
	{
		// Physics
		constexpr int32_t velocityIterations = 6;
		constexpr int32_t positionIterations = 2;
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

		// Retrieve transform from Box2D
		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			const auto& position = body->GetPosition();
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
			transform.Rotation.z = body->GetAngle();
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		m_Registry.view<Rigidbody2DComponent>().each([=](auto entity, auto& rb2d)
		{
			if (rb2d.RuntimeBody == nullptr)
			{
				for (size_t i = 0; i < m_CreateB2BodyQueue.size(); i++)
				{
					if (!m_PhysicsWorld->IsLocked())
					{
						CreateBody(m_CreateB2BodyQueue[i]);
						m_CreateB2BodyQueue.erase(m_CreateB2BodyQueue.begin() + i);
					}
				}
			}
		});

		UpdatePhysics(ts);

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			if (m_DrawOrder.empty())
			{
				auto& group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (int i = group.size() - 1; i >= 0; i--)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(group[i]);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)group[i]);
				}
			}
			else 
			{
				for (int j = 0; j < m_DrawOrder.size(); j++)
				{
					if (m_DrawOrder[j].second.HasComponent<SpriteRendererComponent>()) 
					{
						auto& transform = m_DrawOrder[j].second.GetComponent<TransformComponent>().GetTransform();
						auto& sprite = m_DrawOrder[j].second.GetComponent<SpriteRendererComponent>();

						if (sprite.Texture != nullptr)
						{
							Renderer2D::DrawSprite(transform, sprite, m_DrawOrder[j].second);
							continue;
						}

						if(sprite.Color.w == 0.0f && sprite.SubTexture == nullptr)
							continue;

						Renderer2D::DrawSprite(transform, sprite, m_DrawOrder[j].second);
					}
				}
			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}



	Entity Scene::GetEntityByTag(const std::string& _tag)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto& entity : view)
		{
			const auto& tag = view.get<TagComponent>(entity).Tag;
			if (tag == _tag)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		auto view = m_Registry.view<IDComponent>();
		for (auto entity : view)
		{
			const auto& id = view.get<IDComponent>(entity).ID;
			if (id == uuid)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::InstantiateScript(Entity entity)
	{
		auto& nsc = entity.GetComponent<NativeScriptComponent>();
		nsc.Instance = nsc.InstantiateScript();
		nsc.Instance->m_Entity = entity;
		nsc.Instance->OnCreate();
	}

	Entity Scene::CreateEntityWithDrawOrder(int order, const std::string& name /*= std::string()*/) // Ascending order
	{
		Entity entity = CreateEntity(name);
		m_DrawOrder.push_back({ order, entity });
		std::sort(m_DrawOrder.begin(), m_DrawOrder.end(), 
			[](const std::pair<int, Entity>& i, const  std::pair<int, Entity>& j) 
			{
				return (i.first < j.first);
			});
		return entity;
	}

	void Scene::ChangeDrawIndex(int index, Entity entity)
	{
		for (auto& e : m_DrawOrder)
		{
			if (e.second == entity)
			{
				e.first = index;
				break;
			}
		}
	}

	void Scene::CreateBody(Entity entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		bodyDef.angle = transform.Rotation.z;
		
		//Entity* userData = new Entity(entity);
		//m_PhysicsUserData.push_back(userData);
		//bodyDef.userData.pointer = (uintptr_t)userData;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		body->SetEnabled(rb2d.Enabled);
		if (!rb2d.Gravity)
			body->SetGravityScale(0.0f);

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = bc2d.Density;
			fixtureDef.friction = bc2d.Friction;
			fixtureDef.restitution = bc2d.Restitution;
			fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
			fixtureDef.isSensor = bc2d.IsSensor;
			//fixtureDef.filter.categoryBits = bc2d.CategoryBits; // <- Is that category
			//fixtureDef.filter.maskBits = bc2d.MaskBits;		// <- Collides with other categories
			body->CreateFixture(&fixtureDef); 
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

			b2CircleShape circleShape;
			circleShape.m_radius = cc2d.Radius;
			circleShape.m_p = { cc2d.Offset.x * cc2d.Radius, cc2d.Offset.y * cc2d.Radius };

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = cc2d.Density;
			fixtureDef.friction = cc2d.Friction;
			fixtureDef.restitution = cc2d.Restitution;
			fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
			fixtureDef.isSensor = cc2d.IsSensor;
			//fixtureDef.filter.categoryBits = cc2d.CategoryBits; // <- Is that category
			//fixtureDef.filter.maskBits = cc2d.MaskBits;		// <- Collides with other categories
			body->CreateFixture(&fixtureDef);
		}

		rb2d.RuntimeBody = body;
	}

	void Scene::SetContactListener(b2ContactListener* listener)
	{
		m_PhysicsWorld->SetContactListener(listener);
	}

	void Scene::DestroyBody(Entity entity)
	{
		b2Body* rb = (b2Body*)entity.GetComponent<Rigidbody2DComponent>().RuntimeBody;

		// Deleting heap allocated b2UserData
		//for (int i = 0; i < m_PhysicsUserData.size(); i++)
		//{
		//	if (entity == *m_PhysicsUserData[i])
		//	{
		//		delete m_PhysicsUserData[i];
		//		m_PhysicsUserData.erase(m_PhysicsUserData.begin() + i);
		//	}
		//}
		m_PhysicsWorld->DestroyBody(rb);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}
}