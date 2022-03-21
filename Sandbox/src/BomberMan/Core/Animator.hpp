#pragma once

#define NO_PLAYING_ANIMATION -1

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
#include <Hurikan/Renderer/Renderer2D.h>
using namespace Hurikan;

struct Frame
{
	Frame(const glm::vec2& coords, const glm::vec4 color)
		: Coords(coords), Color(color)
	{}

	Frame(const Frame&) = default;

	glm::vec2 Coords;
	glm::vec4 Color;
};

class Animation
{
public:
	Animation() = default;
	Animation(const Animation&) = default;

	void Create(const Ref<Texture2D>& spritesheet, const glm::vec2& spritesize, const std::vector<Frame>& frames);

	void Reset();
	void OnUpdate(SpriteRendererComponent* src, Timestep ts);
public:
	std::string Tag;
	bool Repeat = false;
	float Delay = 0.0f;
private:
	bool m_Playing = false;
	int m_AnimationIndex = 0;
	float m_Timer = 0.0f;
	std::vector<std::pair<Ref<SubTexture2D>, glm::vec4>> m_Frames; // coords, color

	friend class Animator;
};

class Animator
{
public:
	Animator() {}
	Animator(const Animator&) = default;
	~Animator();
	// Nastavení cílené entity
	void SetTarget(Entity& e);
	// Nastavení barvy animace
	inline void SetColor(const glm::vec4& color) { m_Color = color; }
	void OnUpdate(Timestep ts);
	// Přídání animace
	void Add(const Animation& animation);
	// Správa animací
	void Play(const std::string& tag);
	void Stop();
	void Pause();

	inline std::string GetCurrentAnimation() const
	{
		if (m_PlayingAnimationIndex == NO_PLAYING_ANIMATION)
			return std::string();
		return m_AnimationList[m_PlayingAnimationIndex].Tag;
	}

	// Indikátor jestli je animátor vůbec aktivní
	bool IsAnyPlaying();
	// Indikátor prázdného animátoru
	bool IsEmpty() const { return m_AnimationList.size() == 0; }
private:
	// Pomocná funkce Play funkce
	void Play_Impl(size_t index);
private:
	size_t m_PlayingAnimationIndex = NO_PLAYING_ANIMATION;
	Entity m_TargetEntity;
	glm::vec4 m_Color = glm::vec4(1.0f);
	// Vektor animací
	std::vector<Animation> m_AnimationList = {};
};
