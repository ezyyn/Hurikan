#include "Hurikan.h"

#include "imgui.h"

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox2D.h"
#include "./game/src/ChessApp.h"

class Sandbox : public Hurikan::Application 
{
public:
	Sandbox() : Application("Sandbox App") {
	//	PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	//	PushLayer(new ChessApp());
	}
	~Sandbox() {

	}
};

Hurikan::Application* Hurikan::CreateApplication() {
	return new Sandbox();
}