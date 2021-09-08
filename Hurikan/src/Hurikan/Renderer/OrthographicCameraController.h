#pragma once
#include "Hurikan/Renderer/OrthographicCamera.h"

#include "Hurikan/Core/Timestep.h"

#include "Hurikan/Events/ApplicationEvent.h"
#include "Hurikan/Events/MouseEvent.h"

namespace Hurikan {
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation);// aspectratio * 2 units

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const OrthographicCameraBounds GetBounds() const { return m_CameraBounds; }

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0;
		float m_CameraRotation = 0.0f;
		bool m_Rotation;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 1.0f;
		glm::vec3 m_CameraPosition = {0.0f,0.0f,0.0f};

		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_CameraBounds;
	};
}

