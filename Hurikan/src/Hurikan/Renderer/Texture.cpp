#include "hupch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hurikan {

	Ref<Hurikan::Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<Hurikan::OpenGLTexture2D>(path);
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

	Hurikan::Ref<Hurikan::Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<Hurikan::OpenGLTexture2D>(width,height);
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

}

