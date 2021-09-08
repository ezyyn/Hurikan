#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"

#include "Shader.h"

namespace Hurikan {

	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t w, uint32_t h);

		static void BeginScene(OrthographicCamera& camera); //TODO: scene parameters
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray,const Ref<Shader>& shader,const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMaxtrix;
		};

		static SceneData* m_SceneData;
	};
}

