#pragma once
#include "hupch.h"

#include "Base.h"
#include "Window.h"

#include "Hurikan/Core/Timestep.h"

#include "Hurikan/Events/ApplicationEvent.h"
#include "Hurikan/Core/LayerStack.h"

#include "Hurikan/ImGui/ImGuiLayer.h"

namespace Hurikan {
	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlayer);

		static inline Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};
	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();
}
