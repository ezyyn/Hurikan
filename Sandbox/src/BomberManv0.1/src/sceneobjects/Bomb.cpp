#include "Bomb.h"

#include "../core/GameComponents.h"

Bomb::Bomb(BombProperties props) : m_Properties(props) 
{
	m_FrameAnimator = (FrameAnimator*)m_Properties.BombEntity.GetComponent<NativeScriptComponent>().Instance;

	//BlockAnimation explAnimation;
	//explAnimation.Tag = "BombExplosion";
	//explAnimation.Animation = m_FrameAnimator->GetAnimationByTag("BombTicking").Animation;

	//m_Properties.LeftEntity.AddComponent<NativeScriptComponent>().Bind<FrameAnimator>();
	//FrameAnimator* left_fa = (FrameAnimator*)m_Properties.LeftEntity.GetComponent<NativeScriptComponent>().Instance;
	//left_fa->Init(props.BombEntity.GetComponent<SpriteRendererComponent>().SubTexture->GetTexture(), {128, 128});
	//BlockAnimation ba;
	//ba.Animation = m_FrameAnimator->GetAnimationByTag("BombExplosion").Animation;
	//left_fa->Add(ba);
	/*
	m_SpreadExplosion.reserve(m_Properties.Reach * 4); // casting uint32_t to size_t might be problem idk why tho
	m_SpreadExplosion.emplace_back(m_Properties.LeftEntity);
	m_SpreadExplosion.emplace_back(m_Properties.UpEntity);
	m_SpreadExplosion.emplace_back(m_Properties.RightEntity);
	m_SpreadExplosion.emplace_back(m_Properties.DownEntity);
	*/
}

bool Bomb::Tick(Timestep ts)
{
	switch (m_Properties.State)
	{
		case BombState::UNLIT:
			break;
		case BombState::TICKING:
		{
			if (!m_FrameAnimator->GetAnimationByTag("BombTicking").Active && m_Properties.State != BombState::EDGE) // First part of the explosion
				m_Properties.State = BombState::EDGE; // Waste of a frame
			break;
		}
		case BombState::EDGE: // This is where spreads join
		{
			m_FrameAnimator->Switch("BombExplosion");
			m_Properties.State = BombState::EXPLODING;

			for (auto& entity : m_Properties.SpreadEntities)
			{
#if 1
				switch (entity.GetComponent<EntityTypeComponent>().Type)
				{
					case EntityType::TILE_BOX:
					{
						entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
						entity.GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
						break;
					}
					case EntityType::BOMB_SPREAD_EXPLOSION:
					{
						entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
						FrameAnimator* frameAnimator = (FrameAnimator*)entity.GetComponent<NativeScriptComponent>().Instance;
						frameAnimator->Switch("BombExplosion");
						break;
					}
				default:
					break;
				}
#endif
			}
			break;
		}
		case BombState::EXPLODING:
		{
			if (!m_FrameAnimator->GetAnimationByTag("BombExplosion").Active) // Second part of the explosion is not active anymore -> Done
			{
				m_Properties.State = BombState::DONE;
				return true;
			}
			break;
		}
		case BombState::DONE:
		{
			return true;
		}
	}
	return false;
}

