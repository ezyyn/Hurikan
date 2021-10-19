#pragma once

#include "Hurikan.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Hurikan/Renderer/EditorCamera.h"

namespace Hurikan {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep& deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void UI_Toolbar();
	private:
		Hurikan::Ref<Framebuffer> m_Framebuffer;

		bool m_ViewportFocus = false, m_ViewportHovered = false;

		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize = {0,0};
		glm::vec2 m_ViewportBounds[2] = {};

		Hurikan::Ref<Scene> m_ActiveScene;

		int m_GizmoType = -1; // no gizmo

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		EditorCamera m_EditorCamera;

		// Editor resources

		Ref<Texture2D> m_IconPlay, m_IconStop;
		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;
	};
}