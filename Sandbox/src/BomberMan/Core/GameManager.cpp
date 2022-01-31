#include "GameManager.h"

#include "BomberMan/Core/LevelManager.h"

#include "BomberMan/UI/MainMenu.h"

#include <Hurikan/Renderer/Renderer2D.h>
#include <Hurikan/Renderer/RenderCommand.h>
#include <Hurikan/Core/Application.h>

#include <imgui.h>

void GameManager::OnAttach()
{
	LevelManager::Init();

	m_MainMenu = new MainMenu;

	m_MainMenu->Attach(this);

	m_Game.Init();
	m_MainMenu->Init(); 
}

void GameManager::OnDetach()
{
	delete m_MainMenu;
}

//static Timestep timestep;
void GameManager::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	//timestep = ts;

	switch (m_CurrentScreen)
	{
	case SceneType::MAIN_MENU:
		m_MainMenu->OnUpdate(ts);
		break;
	case SceneType::LOADING_LEVEL:
		break;
	case SceneType::IN_GAME:
		m_Game.OnUpdate(ts);
		break;
	case SceneType::FAIL_LEVEL:
		break;
	case SceneType::SUCCESS_LEVEL:
		break;
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
	//ImGui::Separator();
	//ImGui::Text("Timestep: %f ms", timestep.GetMilliseconds());

	ImGui::End();
}

void GameManager::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN(GameManager::OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(HU_BIND_EVENT_FN(GameManager::OnKeyReleased));
}

static short int key = 0;

bool GameManager::OnKeyPressed(KeyPressedEvent& e)
{
	if (key != Key::Escape && e.GetKeyCode() == Key::Escape)
	{
		Application::Get().Close();
		key = Key::Escape;
	}

	if (key != Key::F5 && e.GetKeyCode() == Key::F5)
	{
		key = Key::F5;
		Application::Get().SetFullScreen(!Application::Get().FullScreenEnabled());
	}

	m_MainMenu->OnKeyPressed(e);
	return false;
}

bool GameManager::OnKeyReleased(KeyReleasedEvent& e)
{
	if (e.GetKeyCode() == Key::F5)
		key = 0;

	return false;
}

void GameManager::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::NEW_GAME_CONFIRMED)
	{
		//! LevelManager::Reset();

		m_Game.Load();
		m_CurrentScreen = SceneType::IN_GAME;
	}
}
