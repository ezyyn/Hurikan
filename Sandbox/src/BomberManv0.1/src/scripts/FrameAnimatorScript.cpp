#include "FrameAnimatorScript.h"

#include "../core/GameComponents.h"

void FrameAnimator::OnCreate()
{
	HU_CORE_ASSERT(HasComponent<SpriteRendererComponent>(), "Entity has no SpriteRendererComponent!");
	HU_CORE_ASSERT(HasComponent<AnimationComponent>(), "Entity has no AnimationComponent!");
}

void FrameAnimator::OnDestroy()
{
}

void FrameAnimator::OnUpdate(Timestep ts)
{
	auto& src = GetComponent<SpriteRendererComponent>();
	auto& ac = GetComponent<AnimationComponent>();

	for (auto& block : ac.Blocks)
	{
		if (block.Active)
		{
			block.OnUpdate(src, ts);
			break;
		}
	}
}

void FrameAnimator::Add(const BlockAnimation& ba)
{
	auto& ac = GetComponent<AnimationComponent>();

	for (auto& block : ac.Blocks)
	{
		if (block.Tag == ba.Tag)
		{
			int line = __LINE__ + 1;
			HU_ERROR("BlockAnimation was not added due to same name! Line: {0}", line);
			return;
		}
	}

	ac.Blocks.push_back(ba);
}

void FrameAnimator::Switch(const std::string& tag)
{
	auto& ac = GetComponent<AnimationComponent>();

	for (auto& block : ac.Blocks)
	{
		block.Active = strcmp(block.Tag.c_str(), tag.c_str()) == 0;
	}
}

BlockAnimation& FrameAnimator::GetActiveAnimation()
{
	auto& ac = GetComponent<AnimationComponent>();

	for (auto& block : ac.Blocks)
	{
		if (block.Active)
			return block;
	}
	HU_WARN("FrameAnimator does not have any active animations!");
	return BlockAnimation{};
}

BlockAnimation& FrameAnimator::GetAnimationByTag(const std::string& tag)
{
	auto& ac = GetComponent<AnimationComponent>();

	for (auto& block : ac.Blocks)
	{
		if (block.Tag == tag)
			return block;
	}
	HU_CORE_ASSERT(false, "");
	return BlockAnimation{};
}