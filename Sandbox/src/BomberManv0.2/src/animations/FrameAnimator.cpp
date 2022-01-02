#include "FrameAnimator.h"

void FrameAnimator::SetTarget(Entity target)
{
	m_Target = target;
}

void FrameAnimator::Stop()
{
	for (auto& anim : m_Animations)
	{
		anim.Index = 0;
		anim.IsPlaying = false;
		anim.m_CurrentTime = 0.0f;
	}
	return;
}

bool FrameAnimator::IsAnyPlaying()
{
	for (auto& anim : m_Animations)
	{
		if (anim.IsPlaying)
			return true;
	}

	return false;
}

void FrameAnimator::Add(const AnimationDetails& details)
{
	m_Animations.push_back({ details });
}

void FrameAnimator::OnUpdate(Timestep ts)
{
	for (auto& anim : m_Animations)
	{
		if (anim.IsPlaying)
		{
			const Animation::Frame* currentFrame = anim.OnUpdate(ts);

			if (currentFrame == nullptr)
			{
				m_TargetSRC->SubTexture = nullptr;
				m_TargetSRC->Color = glm::vec4(0.0f);
				break;
			}

			m_TargetSRC->SubTexture = currentFrame->Subtexture;
			m_TargetSRC->Color = currentFrame->Color;
			break;
		}
	}
}

void FrameAnimator::Play(const std::string& tag)
{
	Stop();

	for (size_t i = 0; i < m_Animations.size(); i++)
	{
		if (strcmp(m_Animations[i].Details.Tag.c_str(), tag.c_str()) == 0)
		{
			//m_PlayingAnimation = &m_Animations[i];
			m_Animations[i].IsPlaying = true;
			m_TargetSRC = &m_Target.GetComponent<SpriteRendererComponent>();
			return;
		}
	}

	HU_ERROR("Animation not found ({0})", tag);
}

void FrameAnimator::Resume()
{
	HU_CORE_ASSERT(false, "Not implemented yet.");
}

void FrameAnimator::Pause()
{
	for (auto& anim : m_Animations)
	{
		if (anim.IsPlaying)
		{
			anim.IsPlaying = false;
			return;
		}
	}
}
