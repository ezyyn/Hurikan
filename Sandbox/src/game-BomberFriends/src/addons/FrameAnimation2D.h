#pragma once
#include <Hurikan.h>

namespace Hurikan
{
	class FrameAnimation2D
	{
	public:
		template<std::size_t SIZE>
		FrameAnimation2D(std::array<Ref<Texture2D>, SIZE> textures);

		void AddFrame(Ref<Texture2D> texture_frame);
		
		void OnRender(glm::vec2& position, glm::vec2& size);
		
		void Play();
		void Pause();
		void Reset();
	private:
		bool m_AnimationPause;
		uint32_t m_AnimationIndex;
		std::vector<Ref<Texture2D>> m_Textures;
	};

}

