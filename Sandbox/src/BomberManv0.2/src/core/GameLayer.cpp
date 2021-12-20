#include "GameLayer.h"

#include <imgui.h>

GameLayer::GameLayer()
	: Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
	m_SceneManager.Init();
}
void GameLayer::OnDetach()
{
	m_SceneManager.Shutdown();
}
float currentTs;
void GameLayer::OnUpdate(Timestep& ts)
{
	currentTs = ts.GetMilliseconds();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	m_SceneManager.OnUpdate(ts);
}
void GameLayer::OnImGuiRender()
{
	// Debug only

	auto stats = Renderer2D::GetStats();
	ImGui::Begin("Stats");
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Timestep: %f", currentTs);
	ImGui::End();

}
void GameLayer::OnEvent(Event& e)
{
	m_SceneManager.OnEvent(e);
}