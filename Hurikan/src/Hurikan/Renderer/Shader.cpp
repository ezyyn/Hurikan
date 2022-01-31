#include "hupch.h"
#include "Shader.h"

#include "Hurikan/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hurikan 
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex, const std::string& fragment)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HU_CORE_ASSERT(false, "RendererAPI: None is an invalid API!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertex, fragment);
		case RendererAPI::API::Vulkan:
			HU_CORE_ASSERT(false, "RendererAPI: Vulkan currently not supported!");
		}

		HU_CORE_ASSERT(false, "RendererAPI: Unknown Renderer API!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HU_CORE_ASSERT(m_Shaders.find((name)) == m_Shaders.end(), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		HU_CORE_ASSERT(m_Shaders.find((name)) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

}
