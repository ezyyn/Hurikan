#include "hupch.h"

#include "Windows_Window.h"

#include "Hurikan/Events/KeyEvent.h"
#include "Hurikan/Events/MouseEvent.h"
#include "Hurikan/Events/MouseEvent.h"
#include "Hurikan/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Hurikan 
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* desc) 
	{
		HU_CORE_ERROR("GLFW Error ({0}): {1}", error, desc);
	}

	Scope<Window> Window::Create(const WindowProps& props) 
	{
		return CreateScope<Windows_Window>(props);
	}

	Windows_Window::Windows_Window(const WindowProps& props) 
	{
		Init(props);
	}

	Windows_Window::~Windows_Window() 
	{
		Shutdown();
	}

	void Windows_Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		delete m_Context;
		m_Context = nullptr;
	}

	void Windows_Window::OnUpdate()
	{
		HU_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	void Windows_Window::SetVSync(bool enabled)
	{
		HU_PROFILE_FUNCTION();

		if (enabled)glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}
	bool Windows_Window::IsVSyncEnabled() const
	{
		return m_Data.VSync;
	}

	void Windows_Window::SetFullScreen(bool fullscreen)
	{
		if (m_Data.FullScreen == fullscreen)
			return;

		m_Data.FullScreen = fullscreen;

		if (m_Data.FullScreen)
		{
			glfwGetWindowPos(m_Window, &m_Data.DefaultPosition[0], &m_Data.DefaultPosition[1]);
			glfwGetWindowSize(m_Window, &m_Data.DefaultSize[0], &m_Data.DefaultSize[1]);

			GLFWmonitor* primary = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primary);

			glfwSetWindowMonitor(m_Window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
			glfwSetWindowMonitor(m_Window, nullptr, m_Data.DefaultPosition[0], m_Data.DefaultPosition[1], m_Data.DefaultSize[0], m_Data.DefaultSize[1], 0);

	}

	bool Windows_Window::FullScreen() const
	{
		return m_Data.FullScreen;
	}

	void Windows_Window::Init(const WindowProps& props)
	{
		HU_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.DefaultSize[0] = props.Width;
		m_Data.DefaultSize[1] = props.Height;
		m_Data.FullScreen = props.FullScreen;

		HU_CORE_INFO("Creating window {0} {1} {2} {3}", props.Title, props.Width, props.Height, props.FullScreen);

		if (!s_GLFWInitialized) 
		{
			HU_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			HU_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// Primary monitor needed for launching in fullscreen
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primary);

		// Centers the window
		m_Data.DefaultPosition[0] = mode->width / 2 - m_Data.Width / 2;
		m_Data.DefaultPosition[1] = mode->height / 2 - m_Data.Height / 2;

		{
			HU_PROFILE_SCOPE("Window Creation");
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			if (props.FullScreen)
			{
				m_Window = glfwCreateWindow(mode->width, mode->height, props.Title.c_str(), primary, nullptr);
			}
			else
			{
				m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
			}
		}

		glfwSetWindowPos(m_Window, m_Data.DefaultPosition[0], m_Data.DefaultPosition[1]);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = w;
			data.Height = h;
			WindowResizeEvent _event(w, h);
			HU_CORE_WARN("{0}, {1}", w, h);
			data.EventCallback(_event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent _event;
			data.EventCallback(_event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window,int key, int scancode,int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action) 
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,int button, int action,int mods) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent _event(keycode);
				data.EventCallback(_event);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window,double xOffset,double yOffset) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent _event((float)xOffset,(float)yOffset);
			data.EventCallback(_event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent _event((float)x, (float)y);
			data.EventCallback(_event);
		});

	}
}
