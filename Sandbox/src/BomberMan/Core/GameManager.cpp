#include "GameManager.hpp"

#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Game/InGame.hpp"

#include <Hurikan/Renderer/Renderer2D.h>
#include <Hurikan/Renderer/RenderCommand.h>
#include <Hurikan/Core/Application.h>

#include <imgui.h>

extern GameData g_InGameData;

void GameManager::OnAttach()
{
	SaveLoadSystem::Init();
	ResourceManager::Init();
	AudioManager::Init();

	m_MainMenu.Attach(this);
	m_MainMenu.Attach(&m_AudioAssistant);
	Attach(&m_AudioAssistant);

	m_MainMenu.Init(); 

	// Load level scene
	{
		{
			const auto& [width, height] = Application::Get().GetWindowSize();

			auto& camera = m_MidScene.CreateEntity();
			auto& camera_cmp = camera.AddComponent<CameraComponent>();
			camera_cmp.Camera.SetViewportSize(width, height);
			camera_cmp.Camera.SetOrthographicSize(10);
			camera_cmp.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		}

		{
			auto& leveltext = m_MidScene.CreateEntity();
			leveltext.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)).SubTexture = ResourceManager::GetSubTexture("LevelText");

			//leveltext.Transform().Translation.x = 2.8f;
			//leveltext.Transform().Translation.y = -1.1f;
			leveltext.Transform().Scale.x *= 1.5f;
			leveltext.Transform().Scale.y *= 0.4f;
		}

		{
			// Level counter, gets data from SaveManager
			m_LevelCount = m_MidScene.CreateEntity();
			m_LevelCount.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)).SubTexture = 
				ResourceManager::GetSubTexture("m" + std::to_string(SaveLoadSystem::GetCurrentLevel().ID));

			m_LevelCount.Transform().Translation.x = 1.0f;
			//levelcount.Transform().Translation.y = -1.1f;
			m_LevelCount.Transform().Scale.x *= 0.6f;
			m_LevelCount.Transform().Scale.y *= 0.6f;
		}
	}

	// Game end text
	{
		m_GameEndText = m_MidScene.CreateEntity();
		ResourceManager::GetTexture("GameEndText");
	}

	m_CurrentScreen = SceneType::MAIN_MENU;
}

void GameManager::OnDetach()
{
	AudioManager::Shutdown();

	if (m_Game)
		delete m_Game;
}

Timestep timestep;

void GameManager::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	timestep = ts;

	switch (m_CurrentScreen)
	{
	case SceneType::MAIN_MENU:
	{
		m_MainMenu.OnUpdate(ts);
		break;
	}
	case SceneType::LOADING_LEVEL:
	{
		LoadLevel(ts);
		break;
	}
	case SceneType::IN_GAME:
	{
		HU_CORE_ASSERT(m_Game, "");
		m_Game->OnUpdate(ts);
		break;
	}
	case SceneType::LEVEL_FAIL:
	case SceneType::LEVEL_SUCCESS:
		WaitAndSwitch(SceneType::LOADING_LEVEL, ts);
		break;

	case SceneType::RETURN_TO_MAIN_MENU:
	{
		delete m_Game;
		m_Game = nullptr;

		m_CurrentScreen = SceneType::MAIN_MENU;
		break;
	}
	case SceneType::GAME_END_TEXT:
	{
		{
			m_Game->OnUpdate(ts);

			static float wait = 3.0f;
			wait -= ts;

			if (wait > 0.0f)
				return;

			wait = 3.0f;
		}

		delete m_Game;
		m_Game = nullptr;
		m_CurrentScreen = SceneType::MAIN_MENU;
		break;
	}
	default:
		break;
	}
}

void GameManager::WaitAndSwitch(SceneType type, Timestep& ts)
{
	{
		m_Game->OnUpdate(ts);

		static float wait = 3.0f;
		wait -= ts;

		if (wait > 0.0f)
			return;

		wait = 3.0f;
	}

	Dispatch(GameEventType::AUDIO_LEVEL_SCREEN);

	delete m_Game;
	m_Game = nullptr;
	m_CurrentScreen = type;
}

void GameManager::LoadLevel(Timestep& ts)
{
	{
		// Display 
		m_MidScene.OnUpdateRuntime(ts);

		static float wait1 = 3.0f;
		wait1 -= ts;

		if (wait1 > 0.0f)
			return;
		
		wait1 = 3.0f;
	}
	m_Game = new InGame;
	m_Game->Attach(this);
	m_Game->Init(m_AudioAssistant);
	//Switch
	m_CurrentScreen = SceneType::IN_GAME;
}

void GameManager::OnImGuiRender()
{
	ImGui::Begin("Stats");

	auto stats = Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Separator();
	ImGui::Text("Timestep: %f ms", timestep.GetMilliseconds());
	ImGui::End();


	ImGui::Begin("GameData");

	ImGui::Text("Completed Levels: %d", g_InGameData.CompletedLevels);
	ImGui::Text("Speed Upgrade: %d", (int)g_InGameData.SpeedUpgrade);
	ImGui::Text("BombCount Upgrade: %d", g_InGameData.BombCountUpgrade);
	ImGui::Text("BombPower Upgrade: %d", g_InGameData.BombPowerUpgrade);
	ImGui::Text("Score: %d", g_InGameData.Score);

	ImGui::End();
}

void GameManager::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN(GameManager::OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(HU_BIND_EVENT_FN(GameManager::OnKeyReleased));
}

bool GameManager::OnKeyPressed(KeyPressedEvent& e)
{
	if (m_CurrentScreen == SceneType::IN_GAME && m_Game)
	{
		if (m_KeyPressed != Key::Escape && e.GetKeyCode() == Key::Escape) // Player cannot pause while level completed or fail -> WORKING
		{
			m_Game->Pause(!m_Game->Paused());

			m_KeyPressed = Key::Escape;
		}
	}

	if (m_KeyPressed != Key::F5 && e.GetKeyCode() == Key::F5)
	{
		m_KeyPressed = Key::F5;
		Application::Get().SetFullScreen(!Application::Get().FullScreenEnabled());
	}

	if(m_CurrentScreen == SceneType::MAIN_MENU)
		m_MainMenu.OnKeyPressed(e);
	
	return false;
}

bool GameManager::OnKeyReleased(KeyReleasedEvent& e)
{
	m_KeyPressed = 0;

	return false;
}

void GameManager::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::GAME_NEW)
	{
		Dispatch(GameEventType::AUDIO_LEVEL_SCREEN);

		SaveLoadSystem::EraseDataAndDeserialize();
		m_CurrentScreen = SceneType::LOADING_LEVEL;
		m_LevelCount.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("m" + std::to_string(SaveLoadSystem::GetCurrentLevel().ID));
	}
	else if (e.Type == GameEventType::GAME_CONTINUE)
	{
		Dispatch(GameEventType::AUDIO_LEVEL_SCREEN);

		m_CurrentScreen = SceneType::LOADING_LEVEL;
	}
	else if (e.Type == GameEventType::GAME_EXIT)
	{
		Application::Get().Close();
	}
	else if (e.Type == GameEventType::LEVEL_FAILED)
	{
		Dispatch(GameEventType::AUDIO_LEVEL_FAILED);

		m_CurrentScreen = SceneType::LEVEL_FAIL;
	}
	else if (e.Type == GameEventType::LEVEL_SUCCESS)
	{
		Dispatch(GameEventType::AUDIO_LEVEL_SUCCESS);

		if (SaveLoadSystem::GetPreviousLevel().BossLevel)
		{
			m_CurrentScreen = SceneType::GAME_END_TEXT;
			return;
		}

		m_CurrentScreen = SceneType::LEVEL_SUCCESS;
		m_LevelCount.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("m" + std::to_string(SaveLoadSystem::GetCurrentLevel().ID));
	}
	else if (e.Type == GameEventType::RETURN_TO_MAIN_MENU)
	{
		// Already dispatched to AudioManager via InGame
		
		m_CurrentScreen = SceneType::RETURN_TO_MAIN_MENU;
		m_LevelCount.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture("m" + std::to_string(SaveLoadSystem::GetCurrentLevel().ID));
		m_MainMenu.UpdateUI();
	}
	else if (e.Type == GameEventType::GAME_COMPLETED)
	{
		// Player finished the game => erase all his progress
		SaveLoadSystem::EraseData();

		m_CurrentScreen = SceneType::GAME_END_TEXT;
	}
}
