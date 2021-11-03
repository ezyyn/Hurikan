#include "Bomb.h"

#include "../core/CustomComponents.h"

Bomb::Bomb(Ref<Scene> scene, Entity& entity, std::vector<Entity> spread_explosion, std::vector<FrameAnimationComponent::FrameData>& bomb_animation)
	: m_Scene(scene), m_BombEntity(entity), m_ExplosionSpread(spread_explosion), m_ExplosionAnimation(bomb_animation)
{
	//auto& nsc = m_BombEntity.GetComponent<NativeScriptComponent>();
	//FrameAnimationComponent* instance = (FrameAnimationComponent*)nsc.Instance;
	//instance->Play();

	m_BombState = BombState::Ticking;
}

bool Bomb::Tick(Timestep ts)
{
	if (m_BombState == BombState::Unlit)
		return false;

	auto& fac = m_BombEntity.GetComponent<FrameAnimationComponent>();
	fac.OnUpdate(ts);

	if (fac.GetAnimationIndex() == 13 && m_BombState != BombState::Exploding)
		m_BombState = BombState::Edge;

	if(m_BombState == BombState::Edge)
	{
		m_BombState = BombState::Exploding;

		m_Scene->ChangeDrawIndex(3, m_BombEntity);

		for (uint32_t j = 0; j < m_ExplosionSpread.size(); j++)
		{
			auto& type = m_ExplosionSpread[j].GetComponent<EntityTypeComponent>().Type;

			switch (type)
			{
			case EntityType::TILE_EMPTY:
			{
				Entity& explosion = m_Scene->CreateEntityWithDrawOrder(3, "BombEntity-SpreadExplosion");

				// Speading the explosion
				explosion.AddComponent<SpriteRendererComponent>();
				explosion.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
				auto& fac = m_ExplosionSpread[j].AddCustomComponent<FrameAnimationComponent>(m_ExplosionSpread[j]);

				explosion.GetComponent<TransformComponent>().Translation = m_ExplosionSpread[j].GetComponent<TransformComponent>().Translation;
				
				fac.SetAnimation(m_ExplosionAnimation);
				fac.Play();

				m_ExplosionEntities.push_back(explosion);
				break;
			}
			case EntityType::TILE_WALL:
			{
				m_ExplosionEntities.push_back(m_ExplosionSpread[j]);
				break;
			}
			case EntityType::TILE_BOX:
			{
				// Animation can be freely added

				m_ExplosionSpread[j].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
				m_ExplosionSpread[j].GetComponent<SpriteRendererComponent>().Texture = nullptr;
				
				m_Scene->DestroyBody(m_ExplosionSpread[j]);
				
				// TODO: move this to animation end
				//m_ExplosionSpread[j].GetComponent<TagComponent>().Tag = "Empty";
				//m_ExplosionSpread[j].GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				auto& fac = m_ExplosionSpread[j].AddCustomComponent<FrameAnimationComponent>(m_ExplosionSpread[j]);

				m_ExplosionSpread[j].RemoveComponent<Rigidbody2DComponent>();
				m_ExplosionSpread[j].RemoveComponent<BoxCollider2DComponent>();

				//fac.SetAnimation(m_ExplosionAnimation);
				//fac.Play();
				m_ExplosionEntities.push_back(m_ExplosionSpread[j]);
				break;
			}

			default:
				HU_CORE_ASSERT(false, "");
				break;
			}
		}

	}

	if (m_BombState == BombState::Exploding)
	{
		for (uint32_t j = 0; j < m_ExplosionEntities.size(); j++)
		{
			auto& type = m_ExplosionEntities[j].GetComponent<EntityTypeComponent>().Type;

			switch (type)
			{
			case EntityType::BOMB_SPREAD_EXPLOSION:
			{
				auto& fac = m_ExplosionEntities[j].GetComponent<FrameAnimationComponent>();
				fac.OnUpdate(ts);
				break;
			}
			case EntityType::TILE_WALL:
			{
				break;
			}
			case EntityType::TILE_BOX:
			{
				auto& fac = m_ExplosionEntities[j].GetComponent<FrameAnimationComponent>();
				fac.OnUpdate(ts);
				break;
			}

			default:
				HU_CORE_ASSERT(false, "Invalid tile!");
				break;
			}
		}
	}
#if 0

	if (!fac.IsAnimationPlaying())
	{
		m_BombState = BombState::Done;

		for (size_t j = 0; j < m_ExplisionEntities.size(); j++)
		{
			m_Scene->DestroyEntity(m_ExplisionEntities[j]);
		}

		m_Scene->DestroyEntity(m_BombEntity);
		return true;
	}
#endif

	return false;
}
