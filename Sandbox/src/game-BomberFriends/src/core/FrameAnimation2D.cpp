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
	// TEMP: 
	int count = 0;

	void FrameAnimation2D::Reset()
	{
		m_AnimationIndex = 0;
		count = 0;
	}
	
	bool FrameAnimation2D::Update(Timestep& ts)
	{
		if (m_Frames.empty())
			return false;
		currentFrameTime += ts;
		if (currentFrameTime >= m_Frames[m_AnimationIndex].Delay)
		{
			currentFrameTime = 0.0f;
			currentFrameTime = (m_AnimationIndex + 1) % m_Frames.size();
		}

		if(m_AnimationPlaying)
			count++;

		if (count == m_Frames[m_AnimationIndex].Delay)
		{
			count = 0;
			m_AnimationIndex++;
		}
	}

	Frame* FrameAnimation2D::CurrentFrame()
	{
		if(m_AnimationIndex < m_Frames.size())
			return &m_Frames[m_AnimationIndex];
		else
		{
			//Reset();
			return nullptr;
		}
	}

}