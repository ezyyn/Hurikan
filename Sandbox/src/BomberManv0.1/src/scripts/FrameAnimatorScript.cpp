#include "FrameAnimatorScript.h"

#include "../core/GameComponents.h"

FrameAnimator::FrameAnimator(Entity target) : m_TargetEntity(target)
{
	HU_CORE_ASSERT(m_TargetEntity.HasComponent<SpriteRendererComponent>(), "Entity has no SpriteRendererComponent!");
}

bool FrameAnimator::IsAnyPlaying()
{
	for (auto& block : m_Blocks)
	{
		if (block.Active)
			return true;
	}
	return false;
}

void FrameAnimator::Stop()
{
	GetActiveAnimation().Active = false;
}

void FrameAnimator::OnUpdate(Timestep ts)
{
	auto& src = m_TargetEntity.GetComponent<SpriteRendererComponent>();

	for (auto& block : m_Blocks)
	{
		if (block.Active)
		{
			block.OnUpdate(src, ts);
			break;
		}
	}
}
void FrameAnimator::Add(const AnimationBlock& ba)
{
	for (auto& block : m_Blocks)
	{
		if (block.Tag == ba.Tag)
		{
			int line = __LINE__ + 1;
			HU_ERROR("BlockAnimation was not added due to same name! Line: {0}", line);
			return;
		}
	}
	
	m_Blocks.push_back(ba);
}

void FrameAnimator::Switch(const std::string& tag)
{
	for (auto& block : m_Blocks)
	{
		block.Active = strcmp(block.Tag.c_str(), tag.c_str()) == 0;
	}
}

AnimationBlock& FrameAnimator::GetActiveAnimation()
{
	for (auto& block : m_Blocks)
	{
		if (block.Active)
			return block;
	}
	// past here frameAnimator does not have any active animations! retuning empty blockanim
	return m_EmptyBA;
}

AnimationBlock FrameAnimator::GetAnimationByTag(const std::string& tag)
{
	for (auto& block : m_Blocks)
	{
		if (block.Tag == tag)
			return block;
	}
	HU_CORE_ASSERT(false, "");
	return AnimationBlock();
}