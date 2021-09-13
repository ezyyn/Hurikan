#include "hupch.h"
#include "OrthographicCameraController.h"

#include "Hurikan/Events/KeyCodes.h"
#include "Hurikan/Events/MouseCodes.h"

#include "Hurikan/Core/Input.h"

namespace Hurikan {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio = 16/9, bool rotation = false)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), /*m_CameraBounds({-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}),*/ m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
	{
		HU_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(Key::Left)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
		}
		else
			if (Input::IsKeyPressed(Key::Right)) {
				m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;
			}  if (Input::IsKeyPressed(Key::Up)) {
				m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;
			}
			else if (Input::IsKeyPressed(Key::Down)) {
				m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;
			}
			m_Camera.SetPosition(m_CameraPosition);
			if (m_Rotation) {
				if (Input::IsKeyPressed(Key::Q)) {
					m_CameraRotation += m_CameraRotationSpeed * deltaTime;
				}
				else if (Input::IsKeyPressed(Key::E)) {
					m_CameraRotation -= m_CameraRotationSpeed * deltaTime;
				}
				m_Camera.SetRotation(m_CameraRotation);
			}
			m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HU_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HU_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HU_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width/height;
	//	m_CameraBounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
	//	m_Camera.SetProjection(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

/*	void OrthographicCameraController::CalculateView()
	{
		m_CameraBounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top);
	}*/

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HU_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * .25f;
		m_ZoomLevel = std::max(m_ZoomLevel, .25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	//	CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		HU_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		//CalculateView();
		return false;
	}

}