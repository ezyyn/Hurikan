#include "GameLayer.h"

#define ROWS 17
#define COLUMNS 17

#include <imgui.h>

#include "../core/AudioManager.h"
#include "../text/GameText.h"

extern size_t usage;

GameLayer::GameLayer(GameApplication* app)
{
	g_App = app;
}

// TODO: TNT maybe ? 
// TODO: FULL SCREEN
void GameLayer::OnAttach()
{
	// Text 
	GameText::Init();
	AudioManager::Init();

	m_GameMenu.Init(g_App->GetWindow().GetWidth(), g_App->GetWindow().GetHeight());

	// Audio stuff
	//AudioManager::Play(AudioType::LEVEL_THEME);
}

void GameLayer::OnDetach()
{
}
void GameLayer::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	m_GameMenu.OnUpdate(ts);

	if (m_GameMenu.Closed())
		g_App->Close();
}
// Player movement slide float
extern float pms = 0.2f;

void GameLayer::OnImGuiRender()
{
	// ImGui is causing textures to flicker ??? TODO: Investigate
#if 0
	auto stats = Renderer2D::GetStats();
	ImGui::Begin("Stats");
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Separator();
	ImGui::DragFloat("drag", &pms, 0.05f, 0.1f, 1.0f);
	//ImGui::Text("Player Health: %d", m_GameManager.m_Player.stats().Health);
#ifdef MEMTRACK
	ImGui::Separator();
	ImGui::Text("Current memory usage: %d bytes", usage);
#endif
	ImGui::End();
#endif 
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN(OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(HU_BIND_EVENT_FN(OnKeyReleased));
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	m_GameMenu.OnKeyPressed(e); 
	return false;
}

bool GameLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	m_GameMenu.OnKeyReleased(e);
	return false;
}

