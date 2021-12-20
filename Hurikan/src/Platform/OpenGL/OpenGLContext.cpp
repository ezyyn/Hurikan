#include "hupch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hurikan 
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		HU_CORE_ASSERT(windowHandle, "Failed to initialize Glad!");
	}

	void OpenGLContext::Init()
	{
		HU_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HU_CORE_ASSERT(status, "Failed to initialize Glad!");
		HU_CORE_INFO("OpenGL Renderer: {0}", glGetString(GL_VERSION));

	#ifdef HU_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		HU_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hurikan requires OpenGL 4.5 or newer!");
	#endif

	}

	void OpenGLContext::SwapBuffers()
	{
		HU_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}