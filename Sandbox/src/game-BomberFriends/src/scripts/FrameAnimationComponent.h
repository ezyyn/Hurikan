#pragma once
#include <Hurikan.h>

namespace Hurikan
{
	class FrameAnimationComponent
	{
	public:
		struct FrameData
		{
			Ref<Texture2D> Texture;
			float Delay = 0.0f;
		};

		FrameAnimationComponent() = default;
		FrameAnimationComponent(const FrameAnimationComponent& other) = default;
		FrameAnimationComponent(Entity entity) { m_Entity = entity; }

		void AddFrame(Ref<Texture2D> texture_frame, float frames);

		void Play();
		void Reset();
		void Stop();
		FrameData* CurrentFrame();

		std::vector<FrameData>& GetAnimation() { return m_Frames; }

		// experimental
		void SelectPartOfAnimation(uint32_t begin, uint32_t end)
		{
			m_BeginIndex = begin;
			m_EndIndex = end;
		}
		void SetAnimation(std::vector<FrameData>& frames) 
		{
			m_Frames.reserve(frames.size());
			for (auto& frame : frames)
				m_Frames.emplace_back(frame);

			m_BeginIndex = 0;
			m_EndIndex = frames.size() - 1;
		}
		void SetAnimationIndex(uint32_t index) { m_AnimationIndex = index; }
		uint32_t GetAnimationIndex() { return m_AnimationIndex; }
		bool IsAnimationPlaying() { return m_AnimationPlaying; }

		void OnCreate();
		void OnDestroy();
		void OnUpdate(Timestep ts);

	private:
		Entity m_Entity;

		uint32_t m_BeginIndex = 0;
		uint32_t m_EndIndex = 0;
		
		bool m_AnimationPlaying = false;
		bool m_AnimationLoop = false;

		float m_CurrentFrameTime = 0;
		uint32_t m_AnimationIndex = 0;
		std::vector<FrameData> m_Frames = {};

	};

}

