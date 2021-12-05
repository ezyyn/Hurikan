#pragma once

#include "Hurikan/Core/Window.h"
#include "Hurikan/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Hurikan 
{
	class Windows_Window : public Window 
	{
	public:
		Windows_Window(const WindowProps& props);
		virtual ~Windows_Window();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSyncEnabled() const override;
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	};
}