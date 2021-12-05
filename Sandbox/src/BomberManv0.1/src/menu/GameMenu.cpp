#include "GameMenu.h"
#include "../text/GameText.h"

void GameMenu::Init(uint32_t width, uint32_t height)
{
	m_MenuCamera.Init(&m_MenuScene, width, height);
	m_MenuScene.OnRuntimeStart();

#if 0
	auto& ent = m_MenuScene.CreateEntityWithDrawOrder(2, "Test");
	ent.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
#endif
	float GameTitleOffsetX = 0.0f;
	float GameTitleOffsetY = 5.0f;

	// BomberMan display text
	GameText::DisplayLetter(&m_MenuScene, 'B', { GameTitleOffsetX - 10.0f,  GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'O', { GameTitleOffsetX - 7.5f, GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'M', { GameTitleOffsetX - 4.5f, GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'B', { GameTitleOffsetX - 1.5f, GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'E', { GameTitleOffsetX + 1.0f,    GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'R', { GameTitleOffsetX + 3.5f,  GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'M', { GameTitleOffsetX + 6.5f,  GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'A', { GameTitleOffsetX + 9.5f,  GameTitleOffsetY }, { 3, 3 });
	GameText::DisplayLetter(&m_MenuScene, 'N', { GameTitleOffsetX + 12.0f,   GameTitleOffsetY }, { 3, 3 });

	// Menu options
	float menuOptionsOffsetX = 0.5f;
	float menuOptionsOffsetY = 4.0f;

	// #NewGame display text
	GameText::DisplayLetter(&m_MenuScene, 'N', {menuOptionsOffsetX + -10.0f,  menuOptionsOffsetY - 4.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'E', {menuOptionsOffsetX + -8.8f,	  menuOptionsOffsetY - 4.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'W', {menuOptionsOffsetX + -7.5f,	  menuOptionsOffsetY - 4.0f }, { 2.0f, 2.0f });
	float offset = -1.3f;
	GameText::DisplayLetter(&m_MenuScene, 'G', { menuOptionsOffsetX + offset + -4.2f,   menuOptionsOffsetY - 4.0f },  { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'A', { menuOptionsOffsetX + offset + -3.0f,   menuOptionsOffsetY - 4.0f },  { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'M', { menuOptionsOffsetX + offset + -1.5f,   menuOptionsOffsetY - 4.0f },  { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'E', { menuOptionsOffsetX + offset +  0.0f,   menuOptionsOffsetY - 4.0f },  { 2.0f, 2.0f });

	//GameText::Display(&m_MenuScene, "Continue", { -10, -6 }, { 1, 0 }, { 1.6f, 1.6f });
	float space = 0.0f;
	// #Continue display text
	GameText::DisplayLetter(&m_MenuScene, 'C', { menuOptionsOffsetX + -10.0f,menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'O', { menuOptionsOffsetX + -8.8f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'N', { menuOptionsOffsetX + -7.6f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -6.4f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'I', { menuOptionsOffsetX + -5.5f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'N', { menuOptionsOffsetX + -4.5f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'U', { menuOptionsOffsetX + -3.3f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'E', { menuOptionsOffsetX + -2.1f, menuOptionsOffsetY - 6.0f }, { 2.0f, 2.0f });

	// #Settings display text
	GameText::DisplayLetter(&m_MenuScene, 'S', { menuOptionsOffsetX + -10.0f,menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'E', { menuOptionsOffsetX + -8.8f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -7.6f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -6.4f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'I', { menuOptionsOffsetX + -5.5f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'N', { menuOptionsOffsetX + -4.5f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'G', { menuOptionsOffsetX + -3.3f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'S', { menuOptionsOffsetX + -2.1f, menuOptionsOffsetY - 8.0f }, { 2.0f, 2.0f });
	// #Statistics display text
	GameText::DisplayLetter(&m_MenuScene, 'S', { menuOptionsOffsetX + -10.0f,menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -8.8f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'A', { menuOptionsOffsetX + -7.6f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -6.4f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'I', { menuOptionsOffsetX + -5.5f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'S', { menuOptionsOffsetX + -4.5f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -3.3f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'I', { menuOptionsOffsetX + -2.4f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'C', { menuOptionsOffsetX + -1.5f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'S', { menuOptionsOffsetX + -0.3f, menuOptionsOffsetY - 10.0f }, { 2.0f, 2.0f });
	// #Exit display text
	GameText::DisplayLetter(&m_MenuScene, 'E', { menuOptionsOffsetX + -10.0f, menuOptionsOffsetY - 12.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'X', { menuOptionsOffsetX + -8.8f,  menuOptionsOffsetY - 12.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'I', { menuOptionsOffsetX + -7.9f,  menuOptionsOffsetY - 12.0f }, { 2.0f, 2.0f });
	GameText::DisplayLetter(&m_MenuScene, 'T', { menuOptionsOffsetX + -7.0f,  menuOptionsOffsetY - 12.0f }, { 2.0f, 2.0f });

	m_ArrowHead = m_MenuScene.CreateEntityWithDrawOrder(0);
	m_ArrowHead.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = Texture2D::Create("assets/textures/heart.png");
	m_ArrowHead.GetComponent<TransformComponent>().Translation.x = -12;
	m_ArrowHead.GetComponent<TransformComponent>().Translation.y = 0.0f;
	m_ArrowHead.GetComponent<TransformComponent>().Rotation.z = glm::radians(90.0f);
}

void GameMenu::OnUpdate(Timestep ts)
{
	switch (m_GameState)
	{
		case GameState::MENU:
			m_MenuScene.OnUpdateRuntime(ts);
			break;
		case GameState::LEVEL_START:
		{
			// TODO: setup level start scene
			// TODO: create GameTimer class
			m_GameState = GameState::INGAME;
			break;
		}
		case GameState::INGAME:
		{
			m_GameManager.OnUpdate(ts);

			if (m_GameManager.GameOver)
				m_GameState = GameState::MENU;
			break;
		}
	}
}

void GameMenu::Confirm()
{
	m_ConfirmedOption = m_SelectedOption;

	switch (m_SelectedOption)
	{
		case GameOptions::NEW_GAME:
		{
			// Resets whole game
			m_GameManager.Init(17, 17);
			// Play the level start scene
			// next is the actual game
			m_GameState = GameState::LEVEL_START;
			break;
		}
	}
}
#ifndef NO_KEY_TYPED 
	#define NO_KEY_TYPED 0
#endif

KeyCode lastKeyPressed = NO_KEY_TYPED;

bool GameMenu::OnKeyPressed(KeyPressedEvent& e)
{
	if (m_GameState == GameState::INGAME)
	{
		m_GameManager.OnKeyPressed(e);
		return false;
	}

	switch (e.GetKeyCode())
	{
		case Key::Enter: Confirm(); break;
		case Key::Up:
			if (lastKeyPressed != Key::Up)
			{
				if (m_SelectedOptionIndex > 0)
				{
					m_SelectedOptionIndex--;
					m_ArrowHead.GetComponent<TransformComponent>().Translation.y += 2;
				}
				m_SelectedOption = (GameOptions)m_SelectedOptionIndex;
			}
			break;
		case Key::Down:
			if (lastKeyPressed != Key::Down)
			{
				if (m_SelectedOptionIndex < 4)
				{
					m_SelectedOptionIndex++;
					m_ArrowHead.GetComponent<TransformComponent>().Translation.y -= 2;
				}
				m_SelectedOption = (GameOptions)m_SelectedOptionIndex;
			}
			break;
	}

	lastKeyPressed = e.GetKeyCode();
	return false;
}

bool GameMenu::OnKeyReleased(KeyReleasedEvent& e)
{
	if (m_GameState == GameState::INGAME)
	{
		m_GameManager.OnKeyReleased(e);
		return false;
	}

	lastKeyPressed = NO_KEY_TYPED;
	return false;
}

