#pragma once

#include "Hurikan.h"

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
		Hurikan::OrthographicCameraController m_CameraController;

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
	};
}