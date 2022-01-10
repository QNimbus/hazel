#pragma once

#include "hazel/renderer/Camera.h"
#include "hazel/renderer/Texture.h"
#include "hazel/renderer/SubTexture2D.h"

#include <glm/glm.hpp>

namespace Hazel {

	class Renderer2D {
	public:
		Renderer2D() = delete;
		Renderer2D(Renderer2D& renderer) = delete;

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, glm::mat4& transform);
		static void EndScene();

		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::mat4& transform, Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, Ref<SubTexture2D>& subTtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		//Statistics
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStatistics();
		static Statistics GetStatistics();

	private:
		static void FlushAndReset();

	};
}