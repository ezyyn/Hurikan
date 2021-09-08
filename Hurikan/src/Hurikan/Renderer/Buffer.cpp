#include "hupch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Hurikan {

	Hurikan::Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false,"RendererAPI: None is an invalid API!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
			break;
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
			break;
		}
	
		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

	Hurikan::Ref<Hurikan::VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
			break;
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
			break;
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

	Hurikan::Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
			break;
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
			break;
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

}