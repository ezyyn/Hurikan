#include "EditorLayer.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Hurikan {
	//temp
	Ref<SubTexture2D> temp_Subtexture;
	//temp--end

	static const uint32_t s_MapWidth = 24;
	static const uint32_t s_MapHeight = 10;
	static const char* s_MapTiles =
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWDDDDWWWWWWWWWWWW"
		"WWWWWWDDDDDDDDWWWWWWWWWW"
		"WWWDDDDDWWWDDDDDDDWWWWWW"
		"WWWDDDDDWWWDDDDDDWWWWWWW"
		"WWWWWWDDDDDDDDDWWWWWWWWW"
		"WWWWWWWDDDDWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		;

	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, false)
	{
	}

	void EditorLayer::OnAttach()
	{
		HU_PROFILE_FUNCTION();

		Hurikan::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;

		m_Framebuffer = Hurikan::Framebuffer::Create(fbSpec);

		m_Texture2D = Hurikan::Texture2D::Create("assets/textures/c.png");

		m_SpriteSheet = Hurikan::Texture2D::Create("assets/textures/tileset.png");

		m_TextureMap['D'] = Hurikan::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6,11 }, { 128,128 });
		m_TextureMap['W'] = Hurikan::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128,128 });

		m_CameraController.SetZoomLevel(5.0f);

		m_ActiveScene = CreateRef<Scene>();

		auto square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{	1.0f, 0.0f, 0.0f, 1.0f });
		m_SquareEntity = square;


		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

		m_SecondCamera = m_ActiveScene->CreateEntity("Second-Camera Entity");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
		cc.Primary = false;   
	}

	void EditorLayer::OnDetach()
	{
		HU_PROFILE_FUNCTION();
	}

#define TEST 2
	void EditorLayer::OnUpdate(Timestep& ts)
	{
		HU_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportFocus)
			m_CameraController.OnUpdate(ts);

		Hurikan::Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		Hurikan::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hurikan::RenderCommand::Clear();
		
#if TEST == 1 
		Hurikan::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (size_t y = 0; y < s_MapHeight; y++)
		{
			for (size_t x = 0; x < s_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * s_MapWidth];
				Ref<SubTexture2D> tile = m_TextureMap[tileType];
				Hurikan::Renderer2D::DrawQuad({ s_MapWidth - x, y, -0.1f }, { 1.0f,1.0f }, tile);
			}
		}
		Hurikan::Renderer2D::EndScene();
#elif TEST == 2
		// Update scene
		m_ActiveScene->OnUpdate(ts);
#endif
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HU_PROFILE_FUNCTION();
		
		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Hurikan::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::Begin("Settings");
		auto stats = Hurikan::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if(m_SquareEntity)
		{
			ImGui::Separator();
			auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			auto& square_color = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(square_color));
			ImGui::Separator();
		}
		ImGui::DragFloat3("Camera Transform",
			glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsAnyWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocus || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize.x != viewportPanelSize.x && m_ViewportSize.y != viewportPanelSize.y)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Hurikan::Event& e)
	{
		//Zommin
		m_CameraController.OnEvent(e);
	}
}