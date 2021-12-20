#pragma once

#include "RendererAPI.h"

namespace Hurikan 
{
	class RenderCommand 
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void RenderCommand::SetViewport(int x, int y, uint32_t w, uint32_t h)
		{
			s_RendererAPI->SetViewport(x, y, w, h);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) 
		{

			uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount; 
			s_RendererAPI->DrawIndexed(vertexArray,count);
		}

		inline static void Shutdown()
		{
			delete s_RendererAPI;
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}