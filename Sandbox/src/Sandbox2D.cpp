#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

//Temporary
#include "ParticleSystem.h"

ParticleSystem temp_ParticleSystem;
ParticleProps temp_Particle;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f,false)
{
}

void Sandbox2D::OnAttach()
{
	HU_PROFILE_FUNCTION();
	m_Texture2D = Hurikan::Texture2D::Create("assets/textures/test_texture.png");

	m_SpriteSheet = Hurikan::Texture2D::Create("assets/textures/tileset.png");
	temp_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	temp_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	temp_Particle.SizeBegin = 0.5f, temp_Particle.SizeVariation = 0.3f, temp_Particle.SizeEnd = 0.0f;
	temp_Particle.LifeTime = 5.0f;
	temp_Particle.Velocity = { 0.0f, 0.0f };
	temp_Particle.VelocityVariation = { 3.0f, 1.0f };
	temp_Particle.Position = { 0.0f, 0.0f };
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
#if TEST==1
	
	{
		static float rotation = 0.0f;
		rotation += ts * 20;

		HU_PROFILE_SCOPE("Sandbox2D:OnUpdate:RendererDraw");
		Hurikan::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hurikan::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f, 0.8f }, { 0.8f,0.2f,0.3f,1.0f });
		Hurikan::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f },  { 0.2f,0.3f,0.8f,1.0f });
		//Hurikan::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f, 0.0f }, { 5.0f,5.0f },45.0f,glm::vec4(1.0f, 0.9f, 0.9f, 0.5f));
		Hurikan::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f,10.0f },glm::radians(rotation),m_Texture2D, 20.0f,glm::vec4(1.0f, 0.9f, 0.9f, 0.5f));

		for (float y = -5.0f; y < 5.0f; y += 0.5f) 
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 rb = { (x + 5.0f) / 10.0f, 0.4f,(y + 5.0f) / 10.0f, 0.5f};
				Hurikan::Renderer2D::DrawQuad({x, y}, { 10.0f,10.0f },rb);
			}
		}

		Hurikan::Renderer2D::EndScene();
	}
#elif TEST == 2
	//Testing particle system
	if (Input::IsMouseButtonPressed(Hurikan::Mouse::Button0))
	{
		auto [x, y] = Input::GetMousePosition();
		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		temp_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			temp_ParticleSystem.Emit(temp_Particle);
	}
	
	temp_ParticleSystem.OnUpdate(ts);
	temp_ParticleSystem.OnRender(m_CameraController.GetCamera());

#elif TEST == 3
	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1,1 }, m_SpriteSheet);
	Renderer2D::EndScene();
#endif
}

void Sandbox2D::OnImGuiRender()
{
	HU_PROFILE_FUNCTION();

	auto stats = Hurikan::Renderer2D::GetStats();

	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
}

void Sandbox2D::OnEvent(Hurikan::Event& e)
{
	//Zommin
	m_CameraController.OnEvent(e);
}