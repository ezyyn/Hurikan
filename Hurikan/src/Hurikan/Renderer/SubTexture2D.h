#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Hurikan
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture,const glm::vec2& min, const glm::vec2& max);

		const glm::vec2* GetTexCoords() const { return m_TexCoords; };
		const Ref<Texture2D> GetTextureCoords() const { return m_Texture; };

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, glm::vec2& coords, glm::vec2& spriteSize);
	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};
}