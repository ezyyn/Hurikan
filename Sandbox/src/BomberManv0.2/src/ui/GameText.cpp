#include "GameText.h"

GameText GameText::s_Instance;

void GameText::InitImpl()
{
	// Loading the spritesheet
	Ref<Texture2D> fontSpriteSheet = Texture2D::Create("assets/textures/font_spritesheet.png");

	// Assigning letters to subtexture
	m_LetterMap['A'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {3, 5}, {20, 20});
	m_LetterMap['B'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {4, 5}, {20, 20});
	m_LetterMap['C'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {5, 5}, {20, 20});
	m_LetterMap['D'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {6, 5}, {20, 20});
	m_LetterMap['E'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {7, 5}, {20, 20});
	m_LetterMap['F'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {8, 5}, {20, 20});
	m_LetterMap['G'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {9, 5}, {20, 20});
	m_LetterMap['H'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {10, 5}, {20, 20});
	m_LetterMap['I'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {11, 5}, {20, 20});
	m_LetterMap['J'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {12, 5}, {20, 20});
	m_LetterMap['K'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {13, 5}, {20, 20});
	m_LetterMap['L'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {14, 5}, {20, 20});
	m_LetterMap['M'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {0, 4}, {20, 20});
	m_LetterMap['N'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {1, 4}, {20, 20});
	m_LetterMap['O'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {2, 4}, {20, 20});
	m_LetterMap['P'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {3, 4}, {20, 20});
	m_LetterMap['Q'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {4, 4}, {20, 20});
	m_LetterMap['R'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {5, 4}, {20, 20});
	m_LetterMap['S'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {6, 4}, {20, 20});
	m_LetterMap['T'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {7, 4}, {20, 20});
	m_LetterMap['U'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {8, 4}, {20, 20});
	m_LetterMap['V'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {9, 4}, {20, 20});
	m_LetterMap['W'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {10, 4}, {20, 20});
	m_LetterMap['X'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {11, 4}, {20, 20});
	m_LetterMap['Y'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {12, 4}, {20, 20});
	m_LetterMap['Z'] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {13, 4}, {20, 20});
	m_LetterMap[' '] = SubTexture2D::CreateFromCoords(fontSpriteSheet, {0, 7}, {20, 20});
}

void GameText::DisplayLetterImpl(Scene* targetScene, char c, const glm::vec2& pos, const glm::vec2& scale)
{
	// Formatting the text to match the letter map
	c = toupper(c);

	// Creating vector of entities representing text
	Entity letter;

	letter = targetScene->CreateEntityWithDrawOrder(m_Order);
	letter.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = m_LetterMap[c];
	letter.GetComponent<TransformComponent>().Translation = { pos.x, pos.y, 0 };
	letter.GetComponent<TransformComponent>().Scale = { scale.x, scale.y, 0 };
}

