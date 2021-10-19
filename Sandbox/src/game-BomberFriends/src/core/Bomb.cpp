#include "Bomb.h"

Bomb::Bomb(Entity& entity, FrameAnimation2D bomb_anim) : m_BombEntity(entity)
{
	std::vector<AnimationComponent::FrameData> parsed;

	for (auto& frame : bomb_anim.GetAnimation())
	{
		parsed.push_back({ frame.Texture,frame.Delay });
	}

	m_BombEntity.AddComponent<SpriteRendererComponent>();
	m_BombEntity.AddComponent<AnimationComponent>().Animation = parsed;
	m_BombEntity.AddComponent<NativeScriptComponent>().Bind<BombScript>();
}


