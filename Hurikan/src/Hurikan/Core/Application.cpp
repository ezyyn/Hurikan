#include "hupch.h"

#include "Application.h"
#include "Hurikan/Core/Log.h"

#include "Hurikan/Renderer/RenderCommand.h"
#include "Hurikan/Renderer/Renderer.h"

#include "Input.h"

#include "GLFW/glfw3.h"

#include "glad/glad.h"

namespace Hurikan {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name = "Hurikan App")
	{
		HU_PROFILE_FUNCTION();

		HU_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(HU_BIND_EVENT_FN(Application::OnEvent));
		//m_Window->SetVSync(false);
		
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
	
	}
	Application::~Application() {
		HU_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		HU_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverLay(Layer* overlayer) {
		HU_PROFILE_FUNCTION();

		m_LayerStack.PushOverLay(overlayer);
		overlayer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		HU_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HU_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HU_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) 
		{
			(*--it)->OnEvent(e);
			if (e.Handled) 
				break;
		}
	}

	void Application::Run() {
		HU_PROFILE_FUNCTION();

		while (m_Running) {
			HU_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime(); // -> Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) 
			{
				{
					HU_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack) {
						layer->OnUpdate(timestep);
					}
				}

				m_ImGuiLayer->Begin();
				{
					HU_PROFILE_SCOPE("Layerstack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HU_PROFILE_FUNCTION();

		m_Minimized = false;
		if (e.GetWidth() == 0 || e.GetHeight() == 0) 
		{
			m_Minimized = true;
			return false;
		}
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}