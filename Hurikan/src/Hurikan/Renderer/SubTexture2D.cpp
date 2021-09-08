#include "hupch.h"

#include "SubTexture2D.h"
namespace Hurikan {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) : m_Texture(texture)
	{
		m_TexCoords[0] = { min.x,min.y };
		m_TexCoords[1] = { max.x,min.y };
		m_TexCoords[2] = { max.x,max.y };
		m_TexCoords[3] = { min.x,max.y };
	}


    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, glm::vec2& coords, glm::vec2& spriteSize)
	{
		constexpr float x = 2, y = 3;
		constexpr float sheetWidth = 2560.0f, sheetHeight = 1664.0f;
		constexpr float spriteWidth = 128.0f, spriteHeight = 128.0f;

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 textureCoords[] = {
			{ (x * spriteWidth) / sheetWidth,(y * spriteHeight) / sheetHeight},
			{ ((x + 1) * spriteWidth) / sheetWidth,(y * spriteHeight) / sheetHeight},
			{ ((x + 1) * spriteWidth) / sheetWidth,((y + 1) * spriteHeight) / sheetHeight},
			{ (x * spriteWidth) / sheetWidth,((y + 1) * spriteHeight) / sheetHeight},
		};

		return CreateRef<SubTexture2D>(texture, coords, spriteSize);
	}

}
