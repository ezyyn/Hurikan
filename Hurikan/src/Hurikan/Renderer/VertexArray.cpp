#include "hupch.h"
#include "VertexArray.h"

#include "Hurikan/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hurikan {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

}