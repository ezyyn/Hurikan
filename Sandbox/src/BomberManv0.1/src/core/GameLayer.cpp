#include "GameLayer.h"

#define ROWS 17
#define COLUMNS 17

#include <imgui.h>

GameLayer::GameLayer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
{
}

void GameLayer::OnAttach()
{	
#if 0
	Ref<SubTexture2D> subtexture = SubTexture2D::CreateFromCoords(spritesheet
		, { 1,1 }, {64, 64});
	auto& ent = m_InGameScene->CreateEntityWithDrawOrder(2, "Test");
	ent.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = subtexture;
#endif
	m_GameCamera.Init(&m_InGameScene, m_Width, m_Height);
	m_GameGrid.Init(&m_InGameScene, ROWS, COLUMNS);
	m_Player.Init(&m_InGameScene, &m_GameGrid);

	m_InGameScene.OnRuntimeStart();
	m_CollisionDetector.Init(&m_InGameScene);
	m_InGameScene.SetContactListener((b2ContactListener*)&m_CollisionDetector);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor({ 0.2f,0.2f,0.2f,1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	m_Player.OnUpdate(ts);
	m_GameGrid.OnUpdate(ts);
	
	// Rendering
	m_InGameScene.OnUpdateRuntime(ts);
}

void GameLayer::OnImGuiRender()
{
	auto stats = Renderer2D::GetStats();
	ImGui::Begin("Stats");
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN(OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(HU_BIND_EVENT_FN(OnKeyReleased));
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	m_Player.OnKeyPressed(e);

	return false;
}

bool GameLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	m_Player.OnKeyReleased(e);

	return false;
}

