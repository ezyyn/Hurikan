#pragma once
#include <Hurikan.h>

namespace Hurikan
{
	struct Frame
	{
		Ref<Texture2D> Texture;
		float Delay = 0.0f;
	};

	class FrameAnimation2D
	{
	public:
		FrameAnimation2D() = default;
		FrameAnimation2D(const FrameAnimation2D& other) = default;

		void AddFrame(Ref<Texture2D> texture_frame, float frames);

		void Play();
		void Reset();
		void Stop();
		bool Update(Timestep& ts);
		Frame* CurrentFrame();

		std::vector<Frame>& GetAnimation() { return m_Frames; }
	private:
		bool m_AnimationPlaying = false;

		float currentFrameTime = 0;
		uint32_t m_AnimationIndex = 0;
		std::vector<Frame> m_Frames = {};
	};

}

