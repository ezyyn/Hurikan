#include "GameLayer.h"

#include <imgui.h>

#define COLUMNS 17
#define ROWS 17

GameLayer::GameLayer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height), m_GameGrid(ROWS, COLUMNS)
{
}

void GameLayer::OnAttach()
{
	m_Scene = CreateRef<Scene>();

//	m_CameraController.SetZoomLevel(9);
//	m_CameraController.SetPosition({ 14, 8, 0 });

	m_GameGrid.Init(m_Scene);
	m_GameCamera.Init(m_Scene->CreateEntity("GameCamera Entity"), m_Width, m_Height);
	m_Player.Init(m_Scene->CreateEntity("Player Entity"));
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	m_Scene->OnUpdateRuntime(ts);
#if 0

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad( { 0, 0 }, {1,1}, { 1.0f, 0.0f, 0.0f, 1.0f });

	m_GameGrid.Update(ts);

	Renderer2D::EndScene();
#endif
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("ddd");
	auto stats = Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void GameLayer::OnEvent(Event& e)
{
}

