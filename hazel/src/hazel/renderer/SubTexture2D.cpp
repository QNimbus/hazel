#include "hzpch.h"
#include "SubTexture2D.h"

namespace Hazel {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& spriteCoords, const glm::vec2& cellSize, const glm::vec2& spriteSize) {
		float spriteWidth = cellSize.x / texture->GetWidth();
		float spriteHeight = cellSize.y / texture->GetHeight();

		glm::vec2 min = { spriteCoords.x * spriteWidth, spriteCoords.y * spriteHeight };
		glm::vec2 max = { min.x + (spriteWidth * spriteSize.x), min.y + (spriteHeight * spriteSize.y) };

		return CreateRef<SubTexture2D>(texture, min, max);
	}
}