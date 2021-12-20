#pragma once

#include "Hurikan/Renderer/RendererAPI.h"

namespace Hurikan {

	class OpenGLRendererAPI : public RendererAPI 
	{
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& VertexArray, uint32_t indexCount = 0) override;
	};

}