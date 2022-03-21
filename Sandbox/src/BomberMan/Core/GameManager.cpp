#include "GameManager.hpp"

#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Game/InGame.hpp"

#include <Hurikan/Renderer/Renderer2D.h>
#include <Hurikan/Renderer/RenderCommand.h>
#include <Hurikan/Core/Application.h>

#include <imgui.h>

static KeyCode s_KeyPressed = 0;

extern GameData g_InGameData;

void GameManager::OnAttach()
{
	// This is where GameManager begins

	// Init of crutial modules
	SaveLoadSystem::Init();
	ResourceManager::Init();
	AudioManager::Init();

	// MainMenu class is attaching its listeners
	m_MainMenu.Attach(this);
	m_MainMenu.Attach(&m_AudioAssistant);

	// AudioAssistant is just a listener that listens to events specified for him. See "Observer.hpp"
	Attach(&m_AudioAssistant);

	m_MainMenu.Init(); 

	// Vytvářím úlohy, které se spustí až bude potřeba

	m_NextLevelTimer.Create(3.0f, [this](float ts) 
		{
			m_Game->OnUpdate((Timestep)ts);
		}, 
		[this](SceneType type)
		{
			Dispatch(GameEventType::AUDIO_LEVEL_SCREEN);
			 
			delete m_Game;
			m_Game = nullptr;
			m_CurrentScreen = type;
			m_NextLevelTimer.Reset();
		});

	m_LoadLevelTimer.Create(3.0f, [this](float ts)
		{
			m_MainMenu.OnUpdateLoadScene((Timestep)ts);
		},
		[this](SceneType type)
		{
			HU_CORE_ASSERT(m_Game == nullptr, "Game is already initialized!");
			m_Game = new InGame;
			m_Game->Attach(this);
			m_Game->Init(m_AudioAssistant);
			m_CurrentScreen = type;
			m_LoadLevelTimer.Reset();
		});


	m_CurrentScreen = SceneType::MAIN_MENU;
}

void GameManager::OnDetach()
{
	// When app ("Application.h") captures that the user is exiting the game it will call this method
	// to ensure that everything gets a chance to be released

	AudioManager::Shutdown();

	if (m_Game)
		delete m_Game;
}

Timestep timestep; // TODO:REMOVE

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
		m_LoadLevelTimer.Tick(ts, SceneType::IN_GAME);
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
	{
		m_NextLevelTimer.Tick(ts, SceneType::LOADING_LEVEL);
		break;
	}
	case SceneType::RETURN_TO_MAIN_MENU:
	{

		delete m_Game;
		m_Game = nullptr;

		m_CurrentScreen = SceneType::MAIN_MENU;
		break;
	}
	case SceneType::GAME_END_TEXT:
	{
		m_NextLevelTimer.Tick(ts, SceneType::MAIN_MENU);
		break;
	}
	default:
		break;
	}
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
		if (s_KeyPressed != Key::Escape && e.GetKeyCode() == Key::Escape) 
		{
			m_Game->Pause(!m_Game->Paused());

			s_KeyPressed = Key::Escape;
		}
	}

	if (s_KeyPressed != Key::F5 && e.GetKeyCode() == Key::F5)
	{
		s_KeyPressed = Key::F5;
		Application::Get().SetFullScreen(!Application::Get().FullScreenEnabled());
		return true;
	}

	if(m_CurrentScreen == SceneType::MAIN_MENU)
		m_MainMenu.OnKeyPressed(e);
	
	return false;
}

bool GameManager::OnKeyReleased(KeyReleasedEvent& e)
{
	s_KeyPressed = 0;

	return true;
}

void GameManager::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::GAME_NEW)
	{
		Dispatch(GameEventType::AUDIO_LEVEL_SCREEN);

		SaveLoadSystem::EraseDataAndDeserialize();
		m_CurrentScreen = SceneType::LOADING_LEVEL;
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

		m_MainMenu.UpdateUI();
	}
	else if (e.Type == GameEventType::RETURN_TO_MAIN_MENU)
	{
		// Already dispatched to AudioManager via InGame
		
		m_CurrentScreen = SceneType::RETURN_TO_MAIN_MENU;

		m_MainMenu.UpdateUI();
	}
	else if (e.Type == GameEventType::GAME_COMPLETED)
	{
		// Player finished the game => erase all his progress
		SaveLoadSystem::EraseData();
		m_MainMenu.UpdateUI();

		m_CurrentScreen = SceneType::GAME_END_TEXT;
	}
}
