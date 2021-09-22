#pragma once

#include "Hurikan/Renderer/Camera.h"

namespace Hurikan
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		float m_OrthographicSize = 10.0;
		float m_OrthographicNear = -1.0, m_OrthographicFar = 1.0;

		float m_AspectRatio = 0.0f;
	};
}