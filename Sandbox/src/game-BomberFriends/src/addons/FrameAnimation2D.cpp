#include "FrameAnimation2D.h"

namespace Hurikan 
{
	template<std::size_t SIZE>
	FrameAnimation2D::FrameAnimation2D(std::array<Ref<Texture2D>, SIZE> textures)
	{
		m_Textures = textures;
	}

	void FrameAnimation2D::AddFrame(Ref<Texture2D> texture_frame)
	{
		m_Textures.push_back(texture_frame);
	}

	void FrameAnimation2D::OnRender(glm::vec2& position, glm::vec2& size)
	{
		while (!m_AnimationPause)
		{
			//Renderer2D::DrawQuad({ position,0 }, size, m_Textures[m_AnimationIndex]);
		}
	}

	void FrameAnimation2D::Play()
	{
		m_AnimationPause = false;
	}

	void FrameAnimation2D::Pause()
	{
		m_AnimationPause = true;
	}

	void FrameAnimation2D::Reset()
	{
		m_AnimationIndex = 0;
		m_AnimationPause = false;
	}

}