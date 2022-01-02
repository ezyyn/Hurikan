#include "FrameAnimator.h"

FrameAnimator::~FrameAnimator()
{
	m_Blocks.clear();
	m_Blocks.shrink_to_fit();
}

bool FrameAnimator::IsAnyPlaying()
{
	return m_ActiveAnimation != nullptr && m_ActiveAnimation->Active;
}

void FrameAnimator::Stop()
{
	if (m_ActiveAnimation != nullptr)
	{
		m_ActiveAnimation->Active = false;
		m_ActiveAnimation->Index = 0;
		m_ActiveAnimation->CurrentFrameTime = 0.0f;
		m_ActiveAnimation = nullptr;
	}
}
void FrameAnimator::Pause()
{
	if (m_ActiveAnimation != nullptr)
	{
		m_ActiveAnimation->Active = false;
	}
}

void FrameAnimator::SetTarget(Entity target)
{
	m_TargetEntity = target;
	m_TargetSRC = &m_TargetEntity.GetComponent<SpriteRendererComponent>();
}

bool FrameAnimator::IsActive(const std::string& tag)
{
	return GetActiveAnimation()->Active;
}

bool FrameAnimator::OnUpdate(Timestep ts)
{
	if (m_ActiveAnimation && m_ActiveAnimation->Active)
	{
		m_ActiveAnimation->OnUpdate(m_TargetSRC, ts);
		return m_ActiveAnimation->Active;
	}

	return false;
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

	size_t newid = m_Blocks.size() - 1;

	HU_INFO(newid);
	m_Blocks[newid].ID = newid;
}

void FrameAnimator::Play(const std::string& tag)
{
	// Reset the animation
	if (m_ActiveAnimation)
	{
		m_ActiveAnimation->CurrentFrameTime = 0.0f;
		m_ActiveAnimation->Active = false;
		m_ActiveAnimation->Index = 0;
	}

	for (size_t i = 0; i < m_Blocks.size(); i++)
	{
		if (strcmp(m_Blocks[i].Tag.c_str(), tag.c_str()) == 0)
		{
			m_ActiveAnimation = &m_Blocks[i];
			m_ActiveAnimation->Active = true;
			return;
		}
	}

	HU_ERROR("Animation not found! (\"{0}\")", tag);
}

AnimationBlock* FrameAnimator::GetActiveAnimation()
{
	if (m_ActiveAnimation == nullptr)
	{
		HU_WARN("No active animation found! Returning empty AnimationBlock.");
	}
	return m_ActiveAnimation;
}

