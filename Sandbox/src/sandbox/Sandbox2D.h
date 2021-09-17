#pragma once

#include "Hurikan.h"

class Sandbox2D : public Hurikan::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hurikan::Timestep deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hurikan::Event& e) override;
private:
	Hurikan::OrthographicCameraController m_CameraController;
	Hurikan::Ref<Hurikan::Texture2D> m_Texture2D;
};

