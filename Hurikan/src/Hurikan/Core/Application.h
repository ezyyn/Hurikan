#pragma once

#include "hupch.h"
#include "Base.h"
#include "Window.h"

#include "Hurikan/Events/ApplicationEvent.h"
#include "Hurikan/Core/LayerStack.h"

#include "Hurikan/ImGui/ImGuiLayer.h"

namespace Hurikan {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			HU_CORE_ASSERT(index < Count, "");
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name, ApplicationCommandLineArgs args = ApplicationCommandLineArgs(), uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application();

		void OnEvent(Event& e);

		static inline Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		void Run();
		void Close();


		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlayer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};
	//TO BE DEFINED IN CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs m_CommandLineArgs);
}
