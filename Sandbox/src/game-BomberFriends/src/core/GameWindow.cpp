#include "GameWindow.h"
#include "imgui.h"

namespace GameFramework 
{
	GameLayer::GameLayer() 
		: Layer("GameFrameworkLayer"), m_CameraController(1280.0f / 720.0f, false),
		m_GameGrid(17, 17),
		m_Player(m_CameraController)
	{
		HU_INFO("Game has started!");
		Init();
	}

	void GameLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");
		auto stats = Hurikan::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();
	}

	void GameLayer::Init()
	{
		m_GameGrid.Init();

		m_CameraController.SetZoomLevel(9);
		m_CameraController.SetPosition({ 14, 8, 0 });

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_Player.LoadBoxProperties(m_GameGrid.GetBoxProperties());
	}

	void GameLayer::OnUpdate(Timestep& ts)
	{
		//m_CameraController.OnUpdate(ts);
		m_GameGrid.Update(ts);
		m_Player.Update(ts);

		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f});
		RenderCommand::Clear();
		Hurikan::Renderer2D::ResetStats();

		Renderer2D::BeginScene(m_EditorCamera);
		
		m_Player.Draw();
		m_GameGrid.Draw();

		Renderer2D::EndScene();
	}

	void GameLayer::OnEvent(Event& e)
	{
		//m_CameraController.OnEvent(e);
	}
}