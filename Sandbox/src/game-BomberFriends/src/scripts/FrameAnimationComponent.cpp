#include "FrameAnimationComponent.h"

namespace Hurikan 
{
	void FrameAnimationComponent::OnCreate()
	{
		m_BeginIndex = 0;
		if (m_Frames.empty())
			m_EndIndex = 0;
		else
			m_EndIndex = m_Frames.size() - 1;
	}

	void FrameAnimationComponent::OnDestroy()
	{
	}

	void FrameAnimationComponent::OnUpdate(Timestep ts)
	{
		if (m_Frames.empty())
			return;

		if (m_AnimationPlaying)
			m_CurrentFrameTime += ts;

		if (m_CurrentFrameTime >= m_Frames[m_AnimationIndex].Delay / 1000)
		{
			m_CurrentFrameTime = 0.0f;
			m_AnimationIndex = (m_AnimationIndex + 1) % m_Frames.size();
		}

		if (m_AnimationIndex == m_EndIndex)
		{
			m_AnimationIndex = m_BeginIndex;
		}

		FrameData* currentFrame = CurrentFrame();

		if (currentFrame == nullptr)
		{
			m_Entity.GetComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f, 1.0f, 0.0f };
			m_AnimationPlaying = false;
			return;
		}
		m_Entity.GetComponent<SpriteRendererComponent>().Texture = currentFrame->Texture;
		m_Entity.GetComponent<SpriteRendererComponent>().Color = { 1.0f,1.0f, 1.0f, 1.0f };
	}

	void FrameAnimationComponent::AddFrame(Ref<Texture2D> texture_frame, float frames)
	{
		m_Frames.push_back({ texture_frame, frames });
	}

	void FrameAnimationComponent::Play()
	{
		m_AnimationPlaying = true;
	}
	void FrameAnimationComponent::Stop()
	{
		m_AnimationPlaying = false;
	}
	void FrameAnimationComponent::Reset()
	{
		m_AnimationIndex = m_BeginIndex;
	}

	FrameAnimationComponent::FrameData* FrameAnimationComponent::CurrentFrame()
	{
		if (m_AnimationIndex < m_EndIndex - 1)
			return &m_Frames[m_AnimationIndex];
		else
		{
			if (!m_AnimationLoop)
			{
				Stop();
				return nullptr;
			}
			else
			{
				Reset();
				return &m_Frames[m_AnimationIndex];
			}
		}
	}
}