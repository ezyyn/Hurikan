#include "MainMenu.h"

#include "BomberMan/Core/ResourceManager.h"
#include "BomberMan/Core/SaveManager.h"
#include "BomberMan/Core/Utils.h"

#include <Hurikan/Core/Application.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>



void MainMenu::Init()
{
	glm::vec4 color;

	if (SaveManager::AlreadyPlayed())
	{
		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		color = glm::vec4(0.0f);
	}


	{
		m_LevelText = m_MenuScene.CreateEntityWithDrawOrder(3);
		m_LevelText.AddComponent<SpriteRendererComponent>(color).SubTexture = ResourceManager::GetSubTexture("LevelText");

		m_LevelText.Transform().Translation.x = 2.8f;
		m_LevelText.Transform().Translation.y = -1.1f;
		m_LevelText.Transform().Scale.x *= 1.5f;
		m_LevelText.Transform().Scale.y *= 0.4f;
	}

	{
		// Level counter, gets data from SaveManager
		m_LevelCount = m_MenuScene.CreateEntityWithDrawOrder(3);
		m_LevelCount.AddComponent<SpriteRendererComponent>(color).SubTexture = ResourceManager::GetSubTexture("m" + std::to_string(SaveManager::GetCurrentLevel().ID));

		m_LevelCount.Transform().Translation.x = 3.8f;
		m_LevelCount.Transform().Translation.y = -1.1f;
		m_LevelCount.Transform().Scale.x *= 0.6f;
		m_LevelCount.Transform().Scale.y *= 0.6f;
	}

	{
		// Arror Head
		m_ArrowHead = m_MenuScene.CreateEntityWithDrawOrder(2);
		m_ArrowHead.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = ResourceManager::GetTexture("ArrowHead");

		m_ArrowHead.Transform().Translation.x = -3.2f;
		m_ArrowHead.Transform().Scale.x *= 0.6f;
		m_ArrowHead.Transform().Scale.y *= 0.6f;
	}

	{
		const auto& [width, height] = Application::Get().GetWindowSize();

		m_Camera = m_MenuScene.CreateEntity();
		auto& camera_cmp = m_Camera.AddComponent<CameraComponent>();
		camera_cmp.Camera.SetViewportSize(width, height);
		camera_cmp.Camera.SetOrthographicSize(10);
		camera_cmp.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	}
	float scale_x = 5.5f;
	float scale_y = 0.5f;
	{
		// New Game
		m_NewGame = m_MenuScene.CreateEntityWithDrawOrder(2);
		m_NewGame.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_NewGame_Text");
		m_NewGame.Transform().Scale.x *= scale_x;
		m_NewGame.Transform().Scale.y *= scale_y;
		// Continue
		m_Continue = m_MenuScene.CreateEntityWithDrawOrder(2);

		glm::vec4 color;
		if (SaveManager::AlreadyPlayed())
			color = glm::vec4(1.0f);
		else
			color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

		m_Continue.AddComponent<SpriteRendererComponent>(color).SubTexture = ResourceManager::GetSubTexture("UI_Continue_Text");
		m_Continue.Transform().Scale.x *= scale_x;
		m_Continue.Transform().Scale.y *= scale_y;
		m_Continue.Transform().Translation.y = -1.0f;
		// Settings
		m_Settings = m_MenuScene.CreateEntityWithDrawOrder(2);
		m_Settings.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Settings_Text");
		m_Settings.Transform().Scale.x *= scale_x;
		m_Settings.Transform().Scale.y *= scale_y;
		m_Settings.Transform().Translation.y = -2.0f;
		// Score
		m_Score = m_MenuScene.CreateEntityWithDrawOrder(2);
		m_Score.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Stats_Text");
		m_Score.Transform().Scale.x *= scale_x;
		m_Score.Transform().Scale.y *= scale_y;
		m_Score.Transform().Translation.y = -3.0f;
		// Exit
		m_Exit = m_MenuScene.CreateEntityWithDrawOrder(2);
		m_Exit.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Exit_Text");
		m_Exit.Transform().Scale.x *= scale_x;
		m_Exit.Transform().Scale.y *= scale_y;
		m_Exit.Transform().Translation.y = -4.0f;
	}
	
	{
		// Background Image
		{
			auto& bckg = m_MenuScene.CreateEntityWithDrawOrder(0);

			auto& src = bckg.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
			src.Texture = ResourceManager::GetTexture("Background2");
			bckg.GetComponent<TransformComponent>().Scale = { 18, 18, 0.0f };
			//bckg.GetComponent<TransformComponent>().Translation = { glm::round(m_CurrentLevel.Width / 2), -glm::round(m_CurrentLevel.Height / 2) - 1, 0 };
			bckg.GetComponent<SpriteRendererComponent>().TilingFactor = 18.0f;
		}
		// Black roundy square
		{
			auto& square = m_MenuScene.CreateEntityWithDrawOrder(1);

			auto& src = square.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
			src.Texture = ResourceManager::GetTexture("Background_UnderText");
			square.Transform().Scale *= 13.0f;
			//square.GetComponent<TransformComponent>().Scale = { 18, 18, 0.0f };
			//bckg.GetComponent<TransformComponent>().Translation = { glm::round(m_CurrentLevel.Width / 2), -glm::round(m_CurrentLevel.Height / 2) - 1, 0 };
			//square.GetComponent<SpriteRendererComponent>().TilingFactor = 18.0f;
		}
	}
	{
		// Logo
		m_Logo = m_MenuScene.CreateEntityWithDrawOrder(2);
		m_Logo.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));

		auto& logo_animator = m_Logo.AddCustomComponent<Animator>();
		logo_animator.Add(ResourceManager::GetAnimation("LogoAnimation"));
		logo_animator.SetTarget(m_Logo);

		logo_animator.Play("LogoAnimation");
		m_Logo.Transform().Scale.x *= scale_x;
		m_Logo.Transform().Scale *= 2.0f;

		m_Logo.Transform().Translation.y = 2.0f;
	}
}

void MainMenu::OnUpdate(Timestep& ts)
{
	m_Logo.GetComponent<Animator>().OnUpdate(ts);
	m_MenuScene.OnUpdateRuntime(ts);
}

void MainMenu::UpdateUI()
{
	m_LevelCount.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("m" + std::to_string(SaveManager::GetCurrentLevel().ID));
	if (SaveManager::AlreadyPlayed())
	{
		m_Continue.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
		m_LevelText.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m_LevelCount.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
}

void MainMenu::OnKeyPressed(KeyPressedEvent& e)
{
	KeyCode currentKey = e.GetKeyCode();

	if (currentKey == Key::Up)
	{
		if (m_SelectedOption > MenuOption::NEW_GAME)
		{
			if (!SaveManager::AlreadyPlayed() && m_SelectedOption == MenuOption::SETTINGS)
			{
				m_SelectedOption = (MenuOption)((int)m_SelectedOption - 2);
				m_ArrowHead.Transform().Translation.y+=2;
				return;
			}

			m_SelectedOption = (MenuOption)((int)m_SelectedOption - 1);
			m_ArrowHead.Transform().Translation.y++;
		}
	}
	else if (currentKey == Key::Down)
	{
		if (m_SelectedOption < MenuOption::EXIT)
		{
			if (!SaveManager::AlreadyPlayed() && m_SelectedOption == MenuOption::NEW_GAME)
			{
				m_SelectedOption = (MenuOption)((int)m_SelectedOption + 2);
				m_ArrowHead.Transform().Translation.y -= 2;
				return;
			}

			m_SelectedOption = (MenuOption)((int)m_SelectedOption + 1);
			m_ArrowHead.Transform().Translation.y--;
		}
	}
	else if (currentKey == Key::Enter)
	{
		switch (m_SelectedOption)
		{
		case MenuOption::NEW_GAME:
			// dialog confirm
			Dispatch(GameEventType::NEW_GAME_CONFIRMED);
			SaveManager::Data().AlreadyPlayed = true;
			SaveManager::Save();
			
			UpdateUI();
			break;
		case MenuOption::CONTINUE:
			Dispatch(GameEventType::CONTINUE_CONFIRMED);
			break;
		case MenuOption::SETTINGS:
			//DispatchToAll(GameEventType::CONTINUE_CONFIRMED);
			break;
		case MenuOption::SCORE:
			break;
		case MenuOption::EXIT:
			Dispatch(GameEventType::EXIT_CONFIRMED);
			break;
		default:
			break;
		}
	}
}

