#pragma once

#include "Hurikan.h"

#include "Panels/SceneHierarchyPanel.h"

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

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Hurikan::Ref<Texture2D> m_Texture2D;
		Hurikan::Ref<Texture2D> m_SpriteSheet;
		Hurikan::Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize = {0,0};
		bool m_ViewportFocus = false, m_ViewportHovered = false;

		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

		Hurikan::Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		int m_GizmoType = -1; // no gizmo

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		EditorCamera m_EditorCamera;
	};
}