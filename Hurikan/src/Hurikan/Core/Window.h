#pragma once
#include "hupch.h"

#include "Hurikan/Core/Base.h"
#include "Hurikan/Events/Event.h"

namespace Hurikan {

   struct WindowProps {
		std::string Title;
		uint32_t Width, Height;
		bool FullScreen;

		WindowProps(const std::string& title = "Hurikan Engine", bool fullScreen = false, uint32_t w = 1280, uint32_t h = 720) 
			: Title(title), Width(w), Height(h), FullScreen(fullScreen)
		{
		}
	};
	//Window Interface
	class Window {
	
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}