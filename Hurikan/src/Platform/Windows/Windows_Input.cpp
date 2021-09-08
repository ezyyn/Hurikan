#include "hupch.h"
#include "Windows_Input.h"

#include "Hurikan/Core/Application.h"
#include "GLFW/glfw3.h"
namespace Hurikan {

	Input* Input::s_Instance = new Windows_Input();

	bool Windows_Input::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Windows_Input::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Windows_Input::GetMouseXImpl()
	{
		return (float)GetMousePositionImpl().first;// FIRST -> X
	}

	float Windows_Input::GetMouseYImpl()
	{
		return (float)GetMousePositionImpl().second; // SECOND -> Y
	}

	std::pair<float, float> Windows_Input::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return{ (float)xpos, (float)ypos};
	}

}