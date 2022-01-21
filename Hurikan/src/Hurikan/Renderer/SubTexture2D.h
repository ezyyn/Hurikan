#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Hurikan
{
	class SubTexture2D
	{
	public:
		//SubTexture2D() = default;
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		SubTexture2D(const SubTexture2D& other);

		/*SubTexture2D(SubTexture2D&& subtexture) noexcept
		{
			Swap(*this, subtexture);
		}

		inline SubTexture2D& operator=(SubTexture2D& other)
		{
			Swap(*this, other);

			return *this;
		}

		inline void Swap(SubTexture2D& first, SubTexture2D& second)
		{
			first.m_Texture = second.m_Texture;

			second.m_Texture.reset();
			std::swap(first.m_TexCoords, second.m_TexCoords);
		}*/

		const glm::vec2* GetTexCoords() const { return m_TexCoords; };
		const Ref<Texture2D>& GetTexture() const { return m_Texture; };

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);
		static SubTexture2D CreateFromCoords2(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize); // TODO: make better name
	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];
	};
}