#pragma once

#include "Hurikan.h"
using namespace Hurikan;

// Singleton
class GameText
{
public:
	GameText(const GameText&) = delete;
	~GameText() = default;

	static void Init() { GetInstance().InitImpl(); }

	static void SetDrawOrder(unsigned int order) { GetInstance().m_Order = order; }
	static void DisplayLetter(Scene* targetScene, char c, const glm::vec2& pos, const glm::vec2& scale)
	{ 
		GetInstance().DisplayLetterImpl(targetScene, c, pos, scale);
	}
private:
	GameText() = default;

	static GameText& GetInstance() { return s_Instance; }

	void InitImpl();
	void DisplayLetterImpl(Scene* targetScene, char c, const glm::vec2& pos, const glm::vec2& scale);
private:
	unsigned int m_Order = 0;

	std::unordered_map<char, Ref<SubTexture2D>> m_LetterMap;

	static GameText s_Instance;
};
