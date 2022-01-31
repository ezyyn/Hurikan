#include "MainMenu.h"

#include "BomberMan/Core/ResourceManager.h"

#include <Hurikan/Core/Application.h>
#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>

void MainMenu::Init()
{
	/*const auto& [width, height] = Application::Get().GetWindowSize();

	double mx, my;
	glfwGetCursorPos(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()), &mx, &my);

	double mouse_x = mx / width,
		mouse_y = my / height;

	HU_INFO(mouse_x)
		HU_INFO(mouse_y)*/

	{
		m_ArrowHead = m_MenuScene.CreateEntityWithDrawOrder(3);
		m_ArrowHead.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = ResourceManager::GetTexture("Heart");

		m_ArrowHead.Transform().Translation.x = -3.5f;
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

	{
		float scale_x = 5.5f;
		float scale_y = 0.5f;

		auto& ui_newgame = m_MenuScene.CreateEntityWithDrawOrder(2);
		ui_newgame.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_NewGame_Text");
		ui_newgame.Transform().Scale.x *= scale_x;
		ui_newgame.Transform().Scale.y *= scale_y;

		auto& ui_continue = m_MenuScene.CreateEntityWithDrawOrder(2);
		ui_continue.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Continue_Text");
		ui_continue.Transform().Scale.x *= scale_x;
		ui_continue.Transform().Scale.y *= scale_y;
		ui_continue.Transform().Translation.y = -1.0f;

		auto& ui_settings = m_MenuScene.CreateEntityWithDrawOrder(2);
		ui_settings.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Continue_Text");
		ui_settings.Transform().Scale.x *= scale_x;
		ui_settings.Transform().Scale.y *= scale_y;
		ui_settings.Transform().Translation.y = -2.0f;


		auto& ui_score = m_MenuScene.CreateEntityWithDrawOrder(2);
		ui_score.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Continue_Text");
		ui_score.Transform().Scale.x *= scale_x;
		ui_score.Transform().Scale.y *= scale_y;
		ui_score.Transform().Translation.y = -3.0f;


		auto& ui_exit = m_MenuScene.CreateEntityWithDrawOrder(2);
		ui_exit.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("UI_Continue_Text");
		ui_exit.Transform().Scale.x *= scale_x;
		ui_exit.Transform().Scale.y *= scale_y;
		ui_exit.Transform().Translation.y = -4.0f;

		auto& ui_bm_logo = m_MenuScene.CreateEntityWithDrawOrder(2);
		ui_bm_logo.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = Texture2D::Create("assets/textures/bomberman_logo.png");
		ui_bm_logo.Transform().Scale.x *= scale_x;
		ui_bm_logo.Transform().Scale *= 2.0f;

		ui_bm_logo.Transform().Translation.y = 2.0f;
	}
}

void MainMenu::OnUpdate(Timestep& ts)
{
	m_MenuScene.OnUpdateRuntime(ts);
}

void MainMenu::OnKeyPressed(KeyPressedEvent& e)
{
	KeyCode currentKey = e.GetKeyCode();

	if (currentKey == Key::Up)
	{
		if (m_SelectedOption > MenuOption::NEW_GAME)
		{
			m_SelectedOption = (MenuOption)((int)m_SelectedOption - 1);
			m_ArrowHead.Transform().Translation.y++;
		}
	}
	else if (currentKey == Key::Down)
	{
		if (m_SelectedOption < MenuOption::EXIT)
		{
			m_SelectedOption = (MenuOption)((int)m_SelectedOption + 1);
			m_ArrowHead.Transform().Translation.y--;
		}
	}
	else if (currentKey == Key::Enter)
	{
		HU_INFO("{0}", (int)m_SelectedOption);

		switch (m_SelectedOption)
		{
		case MenuOption::NEW_GAME:
			// dialog confirm 
			DispatchToAll(GameEventType::NEW_GAME_CONFIRMED);
			break;
		case MenuOption::CONTINUE:
			break;
		case MenuOption::SETTINGS:
			break;
		case MenuOption::SCORE:
			break;
		case MenuOption::EXIT:
			break;
		default:
			break;
		}
	}
}

