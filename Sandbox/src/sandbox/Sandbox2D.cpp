#include "Sandbox2D.h"
#include "imgui.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f,false)
{
}

void Sandbox2D::OnAttach()
{
	HU_PROFILE_FUNCTION();

	m_Texture2D = Hurikan::Texture2D::Create("assets/textures/c.png");
}

void Sandbox2D::OnDetach()
{
	HU_PROFILE_FUNCTION();
}

#define TEST 2


void Sandbox2D::OnUpdate(Hurikan::Timestep ts)
{
	HU_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		HU_PROFILE_SCOPE("Sandbox2D:OnUpdate:RendererPrep");
		Hurikan::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hurikan::RenderCommand::Clear();
	}
	//Reset Statistics
	Hurikan::Renderer2D::ResetStats();
	// Scene lifetime

	Hurikan::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hurikan::Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, { 1,1 }, {1,1,1,1});
	Hurikan::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	HU_PROFILE_FUNCTION();

	auto stats = Hurikan::Renderer2D::GetStats();

	ImGui::Begin("Settings");
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void Sandbox2D::OnEvent(Hurikan::Event& e)
{
	//Zommin
	m_CameraController.OnEvent(e);
}