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

	struct ApplicationSpecification
	{
		std::string Name = "Hurikan App";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
		bool WindowDecorated = false;
		bool Fullscreen = false;
		bool VSync = true;
		bool StartMaximized = true;
		bool Resizable = true;
		bool EnableImGui = true;
	};

	class Application
	{
	public:
		Application(ApplicationSpecification specification, ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		virtual void OnInit() {}

		void OnEvent(Event& e);

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		void Run();
		void Close();

		void PushLayer(Layer* layer);
		void PushOverLay(Layer* overlayer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

		inline std::pair<uint32_t, uint32_t> GetWindowSize() { return std::make_pair(GetWindow().GetWidth(), GetWindow().GetHeight()); }

		void SetFullScreen(bool fullscreen);
		inline bool FullScreenEnabled() const { return m_Window->FullScreen(); }

	private:
		bool OnWindowClose(WindowCloseEvent& _event);
		bool OnWindowResize(WindowResizeEvent& _event);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		ApplicationSpecification m_Specification;
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
