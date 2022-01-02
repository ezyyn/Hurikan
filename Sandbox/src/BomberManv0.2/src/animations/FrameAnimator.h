#pragma once

#include <Hurikan.h>
using namespace Hurikan;

struct AnimationFrame
{
	glm::vec2 SpritePosition;
	glm::vec4 Color;
	float Delay;
};

struct AnimationDetails
{
	std::string Tag;
	glm::vec2 SpriteSize;
	Ref<Texture2D> SpriteSheet;
	std::vector<AnimationFrame> Frames;
	bool PlayNow = false;
	bool Repeat = false;
};

struct Animation
{
	struct Frame
	{
		Frame(const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float delay) 
			: Subtexture(subtexture), Color(color), Delay(delay)
		{
		}

		Ref<SubTexture2D> Subtexture;
		glm::vec4 Color;
		float Delay;
	};

	Animation(const AnimationDetails& details) : Details(details)
	{
		IsPlaying = details.PlayNow;
		OnRepeat = details.Repeat;

		for (auto& frame : details.Frames)
		{
			m_Animation.push_back({ SubTexture2D::CreateFromCoords(details.SpriteSheet, frame.SpritePosition,
				details.SpriteSize), 
				frame.Color, frame.Delay });
		}
		//m_Animation.push_back({ nullptr, glm::vec4(0.0f), 0.0f });
	}

	inline const Frame* OnUpdate(Timestep ts)
	{
		if (!IsPlaying)
			return nullptr;

		Frame* tobereturned = &m_Animation[Index];

		m_CurrentTime += ts;
		if (m_CurrentTime >= tobereturned->Delay / 1000.0f)
		{
			m_CurrentTime = 0.0f;

			// Last frame
			if (Index == m_Animation.size() - 1 && !OnRepeat)
			{
				IsPlaying = false;

				return nullptr;
			}
			else if (Index == m_Animation.size() - 1 && OnRepeat)
			{
				Index = 0;

				return tobereturned;
			}
			Index++;
		}

		return tobereturned;
	}
	
	bool IsPlaying = false;
	bool OnRepeat = false;

	std::vector<Frame> m_Animation;
	AnimationDetails Details;

	float m_CurrentTime = 0.0f;

	unsigned int Index = 0;
};

class FrameAnimator
{
public:
	FrameAnimator() = default;
	FrameAnimator(const FrameAnimator&) = default;
	~FrameAnimator() = default;

	void SetTarget(Entity target);
	void Add(const AnimationDetails& details);
	void Play(const std::string& tag);
	void Resume();
	void Pause();
	void Stop(); 

	bool IsAnyPlaying();

	void OnUpdate(Timestep ts);
private:
	bool m_IsPlaying = false;

	Entity m_Target;

	SpriteRendererComponent* m_TargetSRC = nullptr;

	std::vector<Animation> m_Animations = {};
};