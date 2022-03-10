#include "Animator.hpp"

////////////////////////////////////////////////////////////////////
///////////////////////  Animation // //////////////////////////////
////////////////////////////////////////////////////////////////////

void Animation::Create(const Ref<Texture2D>& spritesheet, const glm::vec2& spritesize, const std::vector<Frame>& frames)
{
	HU_CORE_ASSERT(frames.size(), "!");
	HU_CORE_ASSERT(spritesheet, "!!");

	m_Frames.reserve(frames.size());
	for (auto& frame : frames)
	{
		m_Frames.emplace_back(SubTexture2D::CreateFromCoords(spritesheet, frame.Coords, spritesize), frame.Color);
	}

	m_Timer = Delay;
}

void Animation::Reset()
{
	m_Playing = false;
	m_Timer = Delay;
	m_AnimationIndex = 0;
}

void Animation::OnUpdate(SpriteRendererComponent* src, Timestep ts)
{
	if (m_Timer >= Delay / 1000.0f) // TODO: Modify
	{
		m_Timer = 0.0f;

		// Last frame
		if (m_AnimationIndex == m_Frames.size() && !Repeat)
		{
			Reset();

			return;
		}

		auto& [frame, color] = m_Frames[m_AnimationIndex];
		src->SubTexture.reset();
		src->SubTexture = frame;
		src->Color = m_Frames[m_AnimationIndex].second;

		if (m_AnimationIndex == m_Frames.size() - 1 && Repeat)
		{
			m_AnimationIndex = 0;

			return;
		}

		m_AnimationIndex++;
	}
	m_Timer += ts;
}

////////////////////////////////////////////////////////////////////
/////////////////////// Animator ///////////////////////////////////
////////////////////////////////////////////////////////////////////
Animator::~Animator()
{
	m_AnimationList.clear();
	m_AnimationList.shrink_to_fit();
}

void Animator::SetTarget(Entity& e)
{
	HU_CORE_ASSERT(e.IsValid(), "Entity is not valid");
	HU_CORE_ASSERT(e.HasComponent<SpriteRendererComponent>(), "Entity does not have SRC!");

	m_TargetEntity = e;
}

void Animator::OnUpdate(Timestep ts)
{
	if (m_PlayingAnimationIndex == NO_PLAYING_ANIMATION)
	{
		// No animation is selected
		return;
	}

	HU_CORE_ASSERT(m_TargetEntity, "Entity is not valid");

	if (!m_AnimationList.empty())
	{
		auto& playing_anim = m_AnimationList[m_PlayingAnimationIndex];

		if (playing_anim.m_Playing)
		{
			playing_anim.OnUpdate(&m_TargetEntity.GetComponent<SpriteRendererComponent>(), ts);
			m_TargetEntity.GetComponent<SpriteRendererComponent>().Color = m_Color;
		}
	}
}

void Animator::Add(const Animation& animation)
{
	m_AnimationList.emplace_back(animation);
}

void Animator::Play(size_t index)
{
	HU_CORE_ASSERT(m_TargetEntity, "Entity is not valid!");
	HU_CORE_ASSERT(index < m_AnimationList.size(), "Index out of range!");

	if (m_PlayingAnimationIndex != NO_PLAYING_ANIMATION)
		m_AnimationList[m_PlayingAnimationIndex].Reset();

	m_PlayingAnimationIndex = index;
	m_AnimationList[m_PlayingAnimationIndex].m_Playing = true;
}

void Animator::Play(const std::string& tag)
{
	for (size_t i = 0; i < m_AnimationList.size(); i++)
	{
		if (strcmp(m_AnimationList[i].Tag.c_str(), tag.c_str()) == 0)
		{
			Play(i);
			return;
		}
	}

	HU_ERROR("\"{0}\" does not exist!", tag);
}

void Animator::Stop()
{
	if (m_PlayingAnimationIndex == NO_PLAYING_ANIMATION)
	{
		return;
	}
	HU_CORE_ASSERT(m_TargetEntity.IsValid(), "Entity is not valid");

	m_AnimationList[m_PlayingAnimationIndex].Reset();

	m_PlayingAnimationIndex = NO_PLAYING_ANIMATION;

}

void Animator::Pause()
{
	if (m_PlayingAnimationIndex == NO_PLAYING_ANIMATION)
		return;

	m_AnimationList[m_PlayingAnimationIndex].m_Playing = false;
}

bool Animator::IsAnyPlaying()
{
	for (auto& anim : m_AnimationList)
	{
		if (anim.m_Playing)
			return true;
	}

	return false;
}
