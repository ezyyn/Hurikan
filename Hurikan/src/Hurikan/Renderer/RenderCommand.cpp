#include "hupch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hurikan 
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}