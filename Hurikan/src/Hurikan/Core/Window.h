#pragma once
#include "hupch.h"

#include "Hurikan/Core/Core.h"
#include "Hurikan/Events/Event.h"

namespace Hurikan {

   struct WindowProps {
		unsigned int Width, Height;
		std::string Title;
		WindowProps(const std::string& title = "Hurikan Engine", unsigned int w = 1280, unsigned h = 720) :Title(title), Width(w), Height(h)
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