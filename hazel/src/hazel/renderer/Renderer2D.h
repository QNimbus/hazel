#pragma once

#include "hazel/renderer/Camera.h"
#include "hazel/renderer/Texture.h"

namespace Hazel {

	class Renderer2D {
	public:
		Renderer2D() = delete;
		Renderer2D(Renderer2D& renderer) = delete;

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		/*static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });*/

	private:
	};
}