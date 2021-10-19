#include "GameLayer.h"

#include <imgui.h>

#include "TextureLoader.h"

#define COLUMNS 17
#define ROWS 17

GameLayer::GameLayer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height), m_GameGrid(ROWS, COLUMNS)
{
}

void GameLayer::OnAttach()
{
	m_Scene = CreateRef<Scene>();

	TextureLoader::Init(); 

	// ORDER !!!!!!! no more
	//auto& test = m_Scene->CreateEntity("ddd");
	//test.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	auto& camera = m_Scene->CreateEntity("GameCameraEntity");
	m_GameCamera.Init(camera, m_Width, m_Height);

	m_GameGrid.Init(m_Scene); // must be first due to reverse draw order
	m_Player.Init(m_Scene); 

	m_Scene->OnRuntimeStart();
}

void GameLayer::OnDetach()
{
	m_Scene->OnRuntimeStop();
}

void GameLayer::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	m_Player.OnUpdate(ts);
	m_Scene->OnUpdateRuntime(ts);
#if 0

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad( { 0, 0 }, {1,1}, { 1.0f, 0.0f, 0.0f, 1.0f });

	m_GameGrid.Update(ts);

	Renderer2D::EndScene();
#endif
}

int frames = 0; // To 60

int seconds = 0;
void GameLayer::OnImGuiRender()
{
	frames++;

	if (frames == 60)
	{
		frames = 0;
		seconds++;
	}

	auto& playerPos = m_Scene->GetEntityByTag("PlayerEntity").GetComponent<TransformComponent>().Translation;
	
	ImGui::Begin("Render Stats:");
	auto stats = Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Separator();
	ImGui::Text("Seconds: %d", seconds);
	ImGui::Text("Player X: %f", glm::round(playerPos.x));
	ImGui::Text("Player Y: %f", glm::round(playerPos.y));
	ImGui::Text("Bombs left: %d", m_Player.count);

	ImGui::End();
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN (GameLayer::OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(HU_BIND_EVENT_FN(GameLayer::OnKeyReleased));
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

