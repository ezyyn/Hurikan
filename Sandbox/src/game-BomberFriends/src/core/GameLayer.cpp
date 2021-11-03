#include "GameLayer.h"

#include <imgui.h>

#include "TextureLoader.h"

#include <HazelAudio/src/HazelAudio.h>

#include "CustomComponents.h"

#define COLUMNS 17
#define ROWS 17

// TEMP:
extern std::string assetsFilePath = "src/game-BomberFriends/assets/";

GameLayer::GameLayer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

void GameLayer::OnAttach()
{
	Hazel::Audio::Init();
	TextureLoader::Init();

	auto theme = Hazel::AudioSource::LoadFromFile(assetsFilePath + "audio/level_theme.ogg", false);
	theme.SetGain(0.1f);
	theme.SetLoop(true);
	Hazel::Audio::Play(theme);

	m_Scene = CreateRef<Scene>();

	m_GameGrid = CreateRef<GameGrid>(ROWS, COLUMNS);

	auto& camera = m_Scene->CreateEntity("GameCameraEntity");
	
	camera.AddCustomComponent<EntityTypeComponent>(EntityType::CAMERA);

	m_GameCamera.Init(camera, m_Width, m_Height);
	
	m_GameGrid->Init(m_Scene);
	m_Player.Init(m_Scene, m_GameGrid); 

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

	m_Scene->OnUpdateRuntime(ts);
	m_Player.OnUpdate(ts);
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
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN(GameLayer::OnKeyPressed));
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

