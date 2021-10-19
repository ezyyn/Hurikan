#include "FrameAnimation2D.h"

namespace Hurikan 
{
	void FrameAnimation2D::AddFrame(Ref<Texture2D> texture_frame, float frames)
	{
		m_Frames.push_back({ texture_frame, frames });
	}

	void FrameAnimation2D::Play()
	{
		m_AnimationPlaying = true;
	}
	void FrameAnimation2D::Stop()
	{
		m_AnimationPlaying = false;
	}
	void FrameAnimation2D::Reset()
	{
		m_AnimationIndex = 0;
	}

	bool FrameAnimation2D::Update(Timestep& ts)
	{
		if (m_Frames.empty())
			return false;

		if (m_AnimationPlaying)
			m_CurrentFrameTime += ts;

		if (m_CurrentFrameTime >= m_Frames[m_AnimationIndex].Delay / 1000)
		{
			m_CurrentFrameTime = 0.0f;
			m_AnimationIndex = (m_AnimationIndex + 1) % m_Frames.size();
		}
	}

	Frame* FrameAnimation2D::CurrentFrame()
	{
		if (m_AnimationIndex < m_Frames.size() - 1)
			return &m_Frames[m_AnimationIndex];
		else
		{
			Stop();
			return nullptr;
		}
	}

}