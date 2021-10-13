#include "GameLayer.h"


GameLayer::GameLayer(uint32_t width, uint32_t height) : m_CameraController(1280.0f / 720.0f, false), m_GameGrid(17,17)
{
}

void GameLayer::OnAttach()
{
	m_CameraController.SetZoomLevel(9);
	m_CameraController.SetPosition({ 14, 8, 0 });
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Timestep& ts)
{
	//m_CameraController.OnUpdate(ts);

	RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	RenderCommand::Clear();
	Hurikan::Renderer2D::ResetStats();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad( { 0, 0 }, {1,1}, { 1.0f, 0.0f, 0.0f, 1.0f });

	m_GameGrid.Update(ts);

	Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{

}

void GameLayer::OnEvent(Event& e)
{

}

