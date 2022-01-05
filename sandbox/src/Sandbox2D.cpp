#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach() {
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStatistics();

	ImGui::Text("Renderer2D Statistics:");
	ImGui::Text("Draw calls: %d", stats.DrawCalls);
	ImGui::Text("Quad count: %d", stats.QuadCount);
	ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Index count: %d", stats.GetTotalIndexCount());

	#ifdef TEMP_OLD
	ImGui::ColorEdit4("Square color", glm::value_ptr(this->m_SquareColor));
	#endif
	ImGui::End();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts) {
	HZ_PROFILE_FUNCTION();

	// Update
	{
		HZ_PROFILE_SCOPE("OnUpdate:: CameraController");
		m_CameraController.OnUpdate(ts);
	}

	//Reset statistics
	Hazel::Renderer2D::ResetStatistics();

	{
		HZ_PROFILE_SCOPE("OnUpdate::Render::Prep");

		// Render preparation
		Hazel::RenderCommand::SetClearColor({ 0.07f, 0.07f, 0.07f, 1.0f });
		Hazel::RenderCommand::Clear();

	}
	{
		HZ_PROFILE_SCOPE("OnUpdate::Render::Draw");

		static float rotation = 0.0f;
		rotation += ts * 100.0f;

		// Render draw
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f },  { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(-rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.1f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 15.0f, 15.0f }, m_CheckerboardTexture, 15.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0; y <= 5.0f; y += 0.5f) {
			for (float x = -5.0; x <= 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Hazel::Event& event) {
	// Pass events to camera controller
	m_CameraController.OnEvent(event);
}
