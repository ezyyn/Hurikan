#include "SimpleUI.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/Utils.hpp"

#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>
#include <Hurikan/Core/Application.h>

extern GameData g_InGameData;

void SimpleUI::Init()
{
	// Load values
	m_CurrentTime = SaveLoadSystem::GetCurrentLevel().Time;
	m_ScoreOnScreen = SaveLoadSystem::GetGameData().Score;

	m_AvailableBombs += g_InGameData.BombCountUpgrade;

	// Basic Init
	{
		m_Camera = m_SceneUI.CreateEntity();

		const auto& [width, height] = Application::Get().GetWindowSize();
		m_Camera.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);

		auto& camera_cmp = m_Camera.GetComponent<CameraComponent>();
		auto& camera = camera_cmp.Camera;
		camera.SetOrthographicSize(12);
		camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	}

	constexpr glm::vec2 panel_offset = { 0.0f, 1.05f };

	// Hearts
	for (int i = 0; i < m_HeartEntities.size(); i++)
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("Heart");

		e.Transform().Translation.x = -6.4f + i * 0.8f;
		e.Transform().Translation.y = 4.3f + panel_offset.y;

		e.Transform().Scale *= 0.55f;

		m_HeartEntities[i] = e;
	}
	
	// Background
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(0);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		auto& transform = e.Transform();

		transform.Translation.x = 0;
		transform.Translation.y = 4.0f + panel_offset.y;
		transform.Scale.x = 22.1f;
		transform.Scale.y = 2.1f;

		m_TopPanel = e;
	}

	// Animated head
	{
		m_Head = m_SceneUI.CreateEntityWithDrawOrder(1);
		m_Head.AddComponent<SpriteRendererComponent>(glm::vec4(1.0)).SubTexture = ResourceManager::GetSubTexture("HeadIdle");

		auto& transform = m_Head.Transform();

		transform.Translation.x = -8.6f;
		transform.Translation.y = 4.10f + panel_offset.y;
		transform.Scale.x *= 1.8f;
		transform.Scale.y *= 1.8f;

		auto& animator = m_Head.AddCustomComponent<Animator>();
		animator.SetTarget(m_Head);
		animator.SetDebugTag("PlayerHead");
		animator.Add(ResourceManager::GetAnimation("HeadUpAnimation"));
		animator.Add(ResourceManager::GetAnimation("HeadDownAnimation"));
		animator.Add(ResourceManager::GetAnimation("HeadLeftAnimation"));
	}

	// Text - Score
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = ResourceManager::GetTexture("ScoreText");

		auto& transform = e.Transform();
		transform.Translation.x = 3.2f;
		transform.Translation.y = 4.2f + panel_offset.y;
		transform.Scale.x = 2.25f;
		transform.Scale.y = 0.5f;
	}

	// Numbas
	{
		std::string string_score = std::to_string(g_InGameData.Score);

		string_score.insert(0, m_ScoreCounter.size() - string_score.size(), '0');
		for (int i = 0; i < m_ScoreCounter.size(); ++i)
		{
			auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(std::string(1, string_score[i]));

			auto& transform = e.Transform();
			transform.Translation.x = 3.0f + i * 0.5f;
			transform.Translation.y = 3.5f + panel_offset.y;

			e.Transform().Scale.x = 0.5f;
			e.Transform().Scale.y = 0.5f;

			m_ScoreCounter[i] = e;
		}
	}
	// Clock
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("Clock");

		auto& transform = e.Transform();
		float clock_pos_x = transform.Translation.x = 7.2f;
		float clock_pos_y = transform.Translation.y = 3.9f + panel_offset.y;

		std::string string_timer = std::to_string(SaveLoadSystem::GetCurrentLevel().Time);

		string_timer.insert(0, m_TimerCountDown.size() - string_timer.size(), '0');
		for (size_t i = 0; i < m_TimerCountDown.size(); ++i)
		{
			auto& pos = m_SceneUI.CreateEntityWithDrawOrder(1);
			pos.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(std::string(1, string_timer[i]));

			auto& transform = pos.Transform();
			transform.Translation.x = clock_pos_x + 0.8f + i * 0.5f;
			transform.Translation.y = clock_pos_y;
			transform.Scale.x = 0.5f;
			transform.Scale.y = 0.5f;

			m_TimerCountDown[i] = pos;
		}
	}

	// Bomb Count 
	for (int i = 0; i < m_BombEntities.size(); i++)
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

		auto& color = glm::vec4(0.0f);

		if (i <= g_InGameData.BombCountUpgrade)
		{
			color = glm::vec4(1.0f);
		}

		e.AddComponent<SpriteRendererComponent>(color).SubTexture = ResourceManager::GetSubTexture("BombCount");

		e.Transform().Translation.x = -6.4f + i * 0.8f;
		e.Transform().Translation.y = 3.5f + panel_offset.y;

		e.Transform().Scale *= 0.55f;
		m_BombEntities[i] = e;
	}

	// PauseMenu
	{
		m_PauseMenu = m_SceneUI.CreateEntityWithDrawOrder(2);
		m_PauseMenu.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("PauseMenu");

		auto& transform = m_PauseMenu.Transform();

		transform.Scale *= 8;
		// Arrow head
		m_PauseMenuArrowHead = m_SceneUI.CreateEntityWithDrawOrder(2);
		m_PauseMenuArrowHead.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = ResourceManager::GetSubTexture("ArrowHead");

		m_PauseMenuArrowHead.Transform().Translation.x = -3;
		m_PauseMenuArrowHead.Transform().Translation.y = -0.2f;
		m_PauseMenuArrowHead.Transform().Scale *= 0.5f;
	}

	// Upgrade Bomb Power
	for (int i = 0; i < m_UpgradeBombPower.size(); i++)
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

		auto& color = glm::vec4(0.0f);

		if (i < g_InGameData.BombPowerUpgrade)
		{
			color = glm::vec4(1.0f);
		}

		e.AddComponent<SpriteRendererComponent>(color).SubTexture = ResourceManager::GetSubTexture("PowerUpgrade");

		e.Transform().Translation.x = -1.4f + i * 0.8f;
		e.Transform().Translation.y = 3.5f + panel_offset.y;
		e.Transform().Scale *= 0.55f;
		m_UpgradeBombPower[i] = e;

	}
	// Upgrade Speed
	for (int i = 0; i < 3; i++)
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

		auto& color = glm::vec4(0.0f);

		if (i < g_InGameData.SpeedUpgrade)
		{
			color = glm::vec4(1.0f);
		}

		e.AddComponent<SpriteRendererComponent>(color).SubTexture = ResourceManager::GetSubTexture("SpeedUpgrade");

		e.Transform().Translation.x = -1.4f + i * 0.8f;
		e.Transform().Translation.y = 4.3f + panel_offset.y;
		e.Transform().Scale *= 0.55f;

		m_UpgradeSpeed[i] = e;
	}
}


void SimpleUI::OnUpdate(Timestep& ts, bool paused)
{
	m_SceneUI.OnUpdateRuntime(ts);

	if (!paused)
	{
		auto& head_fa = m_Head.GetComponent<Animator>();

		if (g_InGameData.Score - m_ScoreOnScreen > 0) 
		{
			OnUpdateScore(ts);
		}
		if(!m_GameOver)
			OnUpdateClock(ts);


		if (m_PlayerHit) {

			m_HitPulseColor.g = Utils::Lerp(m_HitPulseColor.g, 0.0f, ts * 2);
			m_HitPulseColor.b = Utils::Lerp(m_HitPulseColor.b, 0.0f, ts * 2);
			head_fa.SetColor(m_HitPulseColor);

			if (m_HitPulseColor.b == 0.0f) {
				m_PlayerHit = false;
			}
		}
		else
		{
			if (m_HitPulseColor.g != 1.0f)
			{
				m_HitPulseColor.g = Utils::Lerp(m_HitPulseColor.g, 1.0f, ts * 2);
				m_HitPulseColor.b = Utils::Lerp(m_HitPulseColor.b, 1.0f, ts * 2);

				head_fa.SetColor(m_HitPulseColor);
			}
		}

		head_fa.OnUpdate(ts);
	} else 
	{
		if (m_LastKey)
		{
			if (Input::IsKeyReleased(Key::Up) && Input::IsKeyReleased(Key::Down) && Input::IsKeyReleased(Key::Enter))
				m_LastKey = 0;
		}

		if (m_LastKey != Key::Enter && Input::IsKeyPressed(Key::Enter))
		{
			m_ConfirmedOption = m_SelectedOption;

			switch (m_ConfirmedOption)
			{
			case PauseMenuOption::MAIN_MENU:
				Dispatch(GameEventType::RETURN_TO_MAIN_MENU_CONFIRMED);
				break;
			case PauseMenuOption::EXIT:
				Application::Get().Close();
				break;
			}
		}

		if (m_LastKey != Key::Up && m_SelectedOption > PauseMenuOption::MAIN_MENU &&  Input::IsKeyPressed(Key::Up))
		{
			m_SelectedOption = (PauseMenuOption)((int)m_SelectedOption - 1);
			m_PauseMenuArrowHead.Transform().Translation.y += 0.9f;
			m_LastKey = Key::Up;
		}
		if (m_LastKey != Key::Down && m_SelectedOption < PauseMenuOption::EXIT && Input::IsKeyPressed(Key::Down))
		{
			m_SelectedOption = (PauseMenuOption)((int)m_SelectedOption + 1);
			m_PauseMenuArrowHead.Transform().Translation.y -= 0.9f;
			m_LastKey = Key::Down;
		}
	}
}
void SimpleUI::OnGameEvent(GameEvent& e)
{
	switch (e.Type)
	{
	case GameEventType::PLAYER_IDLE:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("HeadIdle");;
		m_Head.GetComponent<Animator>().Stop();
		break;
	}

	case GameEventType::PLAYER_DIR_UP:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<Animator>().Play("UpAnimation");
		break;
	}

	case GameEventType::PLAYER_DIR_DOWN:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<Animator>().Play("DownAnimation");
		break;
	}
	case GameEventType::PLAYER_DIR_LEFT:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<Animator>().Play("LeftAnimation");
		break;
	}
	case GameEventType::PLAYER_DIR_RIGHT:
	{
		m_Head.Transform().Scale.x *= -1;
		m_Head.GetComponent<Animator>().Play("LeftAnimation");
		break;
	}
	case GameEventType::DISPLAY_KEY_FIRST:
	{
		// TODO: implement

		break;
	}
	case GameEventType::PLAYER_GONE:
	{
		m_GameOver = true;
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("HeadIdle");
		m_Head.GetComponent<Animator>().Stop();

		for (int i = 0; i < 3; ++i) 
		{
			m_HeartEntities[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
		}
		break;
	}
	case GameEventType::PLAYER_HIT:
	{
		m_PlayerHit = true;
		const auto& health = std::any_cast<int>(e.Data);
		m_HeartEntities[health].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
		break;
	}
	case GameEventType::PLAYER_SUCCESS_EXIT:
	{
		m_GameOver = true;
		/*m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<SpriteRendererComponent>().SubTexture = m_IdleAnimation;
		m_Head.GetComponent<Animator>().Stop();*/
		break;
	}
	case GameEventType::BOMB_PLACED:
	{
		if (m_AvailableBombs != 0) 
		{
			m_BombEntities[m_AvailableBombs - 1].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
			m_AvailableBombs--;
		}
		
		break;
	}
	case GameEventType::BOMB_EXPLODED:
	{
		m_BombEntities[m_AvailableBombs].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
		m_AvailableBombs++;
		break;
	}
	case GameEventType::PLAYER_SPEED_UPGRADE:
	{
		m_UpgradeSpeed[g_InGameData.SpeedUpgrade - 1].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
		break;
	}
	case GameEventType::PLAYER_BOMB_COUNT_UPGRADE:
	{
		if (g_InGameData.BombCountUpgrade < m_BombEntities.size())
		{
			m_AvailableBombs++;

			for (int i = 0; i < m_AvailableBombs; ++i)
			{
				m_BombEntities[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}

		}
		break;
	}
	case GameEventType::PLAYER_POWER_UPGRADE:
	{
		m_UpgradeBombPower[g_InGameData.BombPowerUpgrade - 1].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
		break;
	}
	}
}

void SimpleUI::DisplayPauseMenu()
{
	m_PauseMenu.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
	m_SelectedOption = PauseMenuOption::MAIN_MENU;
	m_ConfirmedOption = PauseMenuOption::NONE;
	m_PauseMenuArrowHead.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
}

void SimpleUI::HidePauseMenu()
{
	m_PauseMenuArrowHead.Transform().Translation.y = -0.2f;

	m_PauseMenu.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
	m_PauseMenuArrowHead.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
}

void SimpleUI::OnUpdateScore(Timestep& ts)
{
	m_ScoreTimer += ts;
	if (m_ScoreTimer >= 0.005f)
	{
		m_ScoreTimer = 0.0f;

		m_ScoreOnScreen += 2;

		auto& string_score = std::to_string(m_ScoreOnScreen);

		string_score.insert(0, 5 - string_score.size(), '0');

		for (int i = 0; i < 5; ++i)
		{
			auto& e = m_ScoreCounter[i];
			e.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(std::string(1, string_score[i]));
		}
	}
}

void SimpleUI::OnUpdateClock(Timestep& ts)
{
	m_ClockTimer += ts;

	if (m_ClockTimer >= 1.0f)
	{
		m_ClockTimer = 0.0f;

		m_CurrentTime--;

		auto& string_timer = std::to_string(m_CurrentTime);

		string_timer.insert(0, 3 - string_timer.size(), '0');
		//HU_INFO(string_timer);
		for (size_t i = 0; i < 3; ++i)
		{
			m_TimerCountDown[i].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(std::string(1, string_timer[i]));
		}

		if (m_CurrentTime == 0)
		{
			// Level failed
			Dispatch(GameEventType::PLAYER_GONE);
		}
	}
}
