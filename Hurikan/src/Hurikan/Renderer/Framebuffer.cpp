#include "hupch.h"
#include "Framebuffer.h"

#include "Hurikan/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hurikan {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
		
	}

}